

	bool ConfigureProject::compress(const std::string& tarf, char * tmp_apidbDir,const std::string& filenameProj)
	{
		TAR *pTar;
       	tar_open(&pTar, (char*)tarf.c_str(), NULL, O_WRONLY | O_CREAT, 0644, TAR_IGNORE_MAGIC);
       	tar_append_tree(pTar, tmp_apidbDir, (char*)filenameProj.c_str());
       	tar_append_eof(pTar);
    	tar_close(pTar);

    	return true;
	}
	bool ConfigureProject::unmcompress(const std::string& filename,char* tmp_apidbDir)
	{
	   	//Descomomprimiendo archivo

        //std::cout << "ConfigureProject::readConfig: Step 4\n";
      	//std::cout << "Descomprimiendo achivo." << std::endl;
		TAR* tar_handle = NULL;
		tar_open(&tar_handle, (char*) filename.c_str(), NULL,  O_RDONLY,  0644,  TAR_GNU);
		if (tmp_apidbDir == NULL)
		{
			//fprintf(stderr, "Failed to build temp file.\n");
				throw core::Error("No se puede crear el directorio tempora para desempauqetar el archivo de proyecto.",ErrorCodes::READFILE_TEMPUNPACKFAIL,__FILE__,__LINE__);

		}
        //std::cout << "ConfigureProject::readConfig: Step 5\n";
  		//std::cout << "tar_handle is " << tmp_apidbDir << std::endl;
		//std::cout << "tmp_filepath "<< tmp_filepath  << std::endl;
     	if(tar_extract_all(tar_handle, tmp_apidbDir) != 0)
      	{
        	core::Error::write( core::Error("Fallo duraten las descompresion del archivo.",ErrorCodes::Read_UncomConfigFile,__FILE__,__LINE__));
         	//std::cout << "Fallo duraten las descompresion del archivo." << std::endl;
      	}
 		tar_close(tar_handle);

 		return true;
	}