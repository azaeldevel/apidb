
#ifdef APIDB_MYSQL
    #include <octetos/db/clientdb-mysql.hh>
    static octetos::db::mysql::Datconnect mysqlSource("localhost",3306,"muposys-0.1-alpha","muposys","123456");
#endif
#ifdef APIDB_POSTGRESQL
    #include <octetos/db/clientdb-postgresql.hh>
    static octetos::db::postgresql::Datconnect postgresqlSource("localhost",5432,"muposys-0.1-alpha","muposys","123456"); 
#endif
#ifdef APIDB_MARIADB
    #include <octetos/db/clientdb-maria.hh>
    static octetos::db::maria::Datconnect mariaSource("localhost",3306,"muposys-0.1-alpha","muposys","123456");
#endif
 
