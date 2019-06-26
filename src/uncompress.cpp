#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <libtar.h>


int main(int argc, char** argv)
    {
                TAR* tar_handle;
                char* tar_fname = "apidb/apidb";
                tar_open(&tar_handle, tar_fname, NULL,  O_RDONLY,  0644,  TAR_GNU);
                //char* savefold = "temp";
                tar_extract_all(tar_handle, "temp");
                tar_close(tar_handle);
                
                return (0);
}
