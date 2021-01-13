
#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include <zip.h>


#include "ConfigureProject.hpp"


namespace octetos
{
namespace apidb
{


static bool is_dir(const std::string& dir)
{
  struct stat st;
  ::stat(dir.c_str(), &st);
  return S_ISDIR(st.st_mode);
}

static void walk_directory(const std::string& startdir, const std::string& inputdir, zip_t *zipper)
{
  DIR *dp = ::opendir(inputdir.c_str());
  if (dp == nullptr) {
    throw std::runtime_error("Failed to open input directory: " + std::string(::strerror(errno)));
  }

  struct dirent *dirp;
  while ((dirp = readdir(dp)) != NULL) {
    if (dirp->d_name != std::string(".") && dirp->d_name != std::string("..")) {
      std::string fullname = inputdir + "/" + dirp->d_name;
      if (is_dir(fullname)) {
        if (zip_dir_add(zipper, fullname.substr(startdir.length() + 1).c_str(), ZIP_FL_ENC_UTF_8) < 0) {
          throw std::runtime_error("Failed to add directory to zip: " + std::string(zip_strerror(zipper)));
        }
        walk_directory(startdir, fullname, zipper);
      } else {
        zip_source_t *source = zip_source_file(zipper, fullname.c_str(), 0, 0);
        if (source == nullptr) {
          throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
        }
        if (zip_file_add(zipper, fullname.substr(startdir.length() + 1).c_str(), source, ZIP_FL_ENC_UTF_8) < 0) {
          zip_source_free(source);
          throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
        }
      }
    }
  }
  ::closedir(dp);
}

static void zip_directory(const std::string& inputdir, const std::string& output_filename)
{
  int errorp;
  zip_t *zipper = zip_open(output_filename.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
  if (zipper == nullptr) {
    zip_error_t ziperror;
    zip_error_init_with_code(&ziperror, errorp);
    throw std::runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
  }

  try {
    walk_directory(inputdir, inputdir, zipper);
  } catch(...) {
    zip_close(zipper);
    throw;
  }

  zip_close(zipper);
}
    
    
    
    
    
    
    
    



bool ConfigureProject::compress(const std::string& tarf, char * tmp_apidbDir,const std::string& filenameProj)
{
	zip_directory(tmp_apidbDir, filenameProj.c_str());
    
    return true;
}








bool ConfigureProject::unmcompress(const std::string& filename,char* tmp_apidbDir)
{
    int err = 0;
    zip *z = zip_open("foo.zip", 0, &err);

    //Search for the file of given name
    const char *name = "file.txt";
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, name, 0, &st);

    //Alloc memory for its uncompressed contents
    char *contents = new char[st.size];

    //Read the compressed file
    zip_file *f = zip_fopen(z, "file.txt", 0);
    zip_fread(f, contents, st.size);
    zip_fclose(f);

    //And close the archive
    zip_close(z);
    if(!std::ofstream("file1.txt").write(contents, st.size))
    {
        std::cerr << "Error writing file" << '\n';
        return EXIT_FAILURE;
    }
    return true;
}


}
}
