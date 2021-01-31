/**
 *  This file is part of apidb.
 *  APIDB do Make easy to connect your Database
 *  Copyright (C) 2018  Azael Reyes
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */

#ifndef APIDB_GENERATOR_HPP
#define APIDB_GENERATOR_HPP

#include <fstream>
#if defined WINDOWS_MINGW
    #include <shell.hh>
#else
    #include <octetos/coreutils/shell.hh>
#endif


namespace octetos
{
namespace apidb
{
	namespace generators
	{
                
        enum OpCode
        {
            NoOp,
            SELECT,
            INSERT,
            UPDATE,
            DELETE,
        };
        
        /**
        * \private No es parte del API
        * */
		class Generator
		{
		public:
            /**
             * \brief Llmada directameten por el driver para inila contruccion
             * */
			virtual bool generate(bool log) = 0;
			OutputLenguajes getOutputLenguaje() const;
			std::string getOutputLenguajeString()const;
			PackingLenguajes getPackingLenguaje() const;
			std::string getPackingLenguajeString()const;
			Generator(const ConfigureProject& config,apidb::Analyzer& analyzer);
            symbols::Symbol* getRootSymbol(symbols::Symbol* k);
            void insertParamsRaw(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent);
            void insertValueRaw(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent);
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            virtual void getKey(std::ofstream& ofile, const symbols::Symbol* k);
            
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            virtual void getKey2(std::ofstream& ofile, const symbols::Symbol* k);
		protected:
			//OutputLenguajes outputLenguaje;//se
			const ConfigureProject& configureProject;
			apidb::Analyzer& analyzer;
			const symbols::SymbolsTable& getSymbolsTable()const;
			coreutils::Shell shell;
		};
        
        class Operation
        {
        public:
            Operation(const ConfigureProject&,const apidb::symbols::Table&,std::ofstream&);
            virtual bool generate() = 0;   
            const char* opConcat() const;
            const char* opReference() const;
            const char* stringType() const;
            const char* integerType() const;
            const char* identifier(const char*) const;
            void setDefinition(bool);
            void setImplementation(bool);
            symbols::Symbol* getRootSymbol(symbols::Symbol* k);
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            virtual void inheritField(std::ofstream& ofile, const symbols::Symbol* k, const char* separator) const;
            
            void insertParamsRaw(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent);
            void insertValueRaw(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent);
            const char* getsqlString()const;
            bool echoKey()const;
            bool echoKeyRawParam()const;
            bool echoDataRawParam(const symbols::Symbol* data)const;
        protected:
            const ConfigureProject& configureProject;
            const apidb::symbols::Table& table;
            std::ofstream& ofile;
            bool definition;
            bool implementation;
        };
        
        class Insert : public Operation
        {          
        public:
            Insert(const ConfigureProject&,const apidb::symbols::Table&,std::ofstream&);
            virtual bool generate();
            
        private:
            bool definite();
            bool implement();
        };

        class Select : public Operation
        {          
        public:
            Select(const ConfigureProject&,const apidb::symbols::Table&,std::ofstream&);
            virtual bool generate();
            
        private:
            bool definite_static();
            bool implement_static();
            bool definite_rawdata();
            bool implement_rawdata();
        };
        
        class Update : public Operation
        {
        public:
            Update(const ConfigureProject&,const apidb::symbols::Table&,std::ofstream&);
            virtual bool generate();
            
        private:
            bool definite();
            bool implement();
        };
        
        class Remove : public Operation
        {
        public:
            Remove(const ConfigureProject&,const apidb::symbols::Table&,std::ofstream&);
            virtual bool generate();
            
        private:
            bool definite();
            bool implement();
        };
        
        class Getter : public Operation
        {
        public:
            Getter(const ConfigureProject&,const apidb::symbols::Table&,std::ofstream&);
            virtual bool generate();
            
        private:
            bool definite();
            bool definite(const symbols::Symbol* s);
            bool definiteKey(const symbols::Symbol* s);
            bool implement();
            bool implement(const symbols::Symbol* s);
            bool implementKey(const symbols::Symbol* s);
        };
        
        //TODO : Impletacion pendiente
        class Download : public Select
        {
            
        };
        
		/**
		* \private
		* */
		class CPP : public Generator
		{
		public:
            /**
             * \brief Llmada directameten por el driver para inila contruccion
             * */
			virtual bool generate(bool log);
			CPP(apidb::Analyzer&,const ConfigureProject&);
			const std::string& getHeaderName() const;
			std::ofstream& getSourceOutput();
			std::ofstream& getHeaderOutput();
			virtual ~CPP();

		private:
            //contructor
			void writeDefaultContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeDefaultContructorCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyRawDataContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyRawDataContructorCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructorCPP(const apidb::symbols::Table&,std::ofstream&);
            
            //selects
			void writeSelectsH(const apidb::symbols::Table& table, std::ofstream& ofile);
			void writeSelectsCPP(const apidb::symbols::Table&,std::ofstream&);
            
            //downloads
			void writeDownloadsH(const apidb::symbols::Table& table, std::ofstream& ofile);
			void writeDownloadsCPP(const apidb::symbols::Table&,std::ofstream&);
            
            //getter
            void writeGettersH(const apidb::symbols::Table& table, std::ofstream& ofile);
            void writeGettersCPP(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //updatter
            void writeUppdatersH(const apidb::symbols::Table& table, std::ofstream& ofile);
            void writeUppdatersCPP(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //inserts
			void writeInsertH(const apidb::symbols::Table&,std::ofstream&);
			void writeInsertCPP(const apidb::symbols::Table&,std::ofstream&);            
            
            //methodes
			void createClassMethodesH(const apidb::symbols::Table&,std::ofstream&);
			void createClassMethodesCPP(const apidb::symbols::Table&,std::ofstream&);            
                        
            //updatter
            void writeRemovesH(const apidb::symbols::Table& table, std::ofstream& ofile);
            void writeRemovesCPP(const apidb::symbols::Table& table, std::ofstream& ofile);
            
			//varias
			bool createH(std::ofstream& file,bool log,const symbols::SymbolsTable&);
			bool createH(std::ofstream& file,bool log,const symbols::ISpace*);
			bool createCPP(std::ofstream& file,bool log,const symbols::SymbolsTable&);
			bool createCPP(std::ofstream& file,bool log,const symbols::ISpace*);
            
            //
			void createClassH(const apidb::symbols::Table&,std::ofstream&,const std::string&,bool log);
            void createClassCPP(const apidb::symbols::Table&,std::ofstream&,const std::string&);
			void createClassPrivateH(std::ofstream&);
			void createClassAttributesH(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublicH(std::ofstream&);			
			void createClassPrivateCPP(std::ofstream&);
			void createClassAttributesCPP(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublicCPP(std::ofstream&);
			void writeKeyValueH(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyValueCPP(const apidb::symbols::Table&,std::ofstream&);
			bool createDatconnectHPP(std::ofstream& file,bool log);
            bool createDatconnectCPP(std::ofstream& file,bool log);
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            //virtual void getKey(std::ofstream& ofile, const symbols::Symbol* k);
            
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            //virtual void getKey2(std::ofstream& ofile, const symbols::Symbol* k);
			//apidb::Analyzer& analyzer;
			std::ofstream* writeResults;//erreglo de writeoutput files
			std::string projectH;
			std::string projectCPP;
		};

        
		class Java : public Generator
		{
		public:
            /**
             * \brief Llmada directameten por el driver para inila contruccion
             * */
			virtual bool generate(bool log);
			Java(apidb::Analyzer&,const ConfigureProject&);
			const std::string& getHeaderName() const;
			//std::ofstream& getSourceOutput();
			//std::ofstream& getHeaderOutput();
			virtual ~Java();
            
		private:
            //contructor
			void writeDefaultContructor(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyRawDataContructor(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructor(const apidb::symbols::Table&,std::ofstream&);
            
            //selects
			void writeSelects(const apidb::symbols::Table&,std::ofstream&);
            //void writeSelectInstancetObjectData(const apidb::symbols::Table&,std::ofstream&);
            
            //downloads
			void writeDownloads(const apidb::symbols::Table&,std::ofstream&);
            
            //getter
            void writeGetters(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //updatter
            void writeUppdaters(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //inserts
			void writeInsert(const apidb::symbols::Table&,std::ofstream&);            
            
            //methodes
			void createClassMethodes(const apidb::symbols::Table&,std::ofstream&);            
            
			//varias
			bool create(bool log,const symbols::SymbolsTable&);
			bool create(bool log,const symbols::ISpace*);
            
            //updatter
            void writeRemoves(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //
            void createClass(const apidb::symbols::Table&,std::ofstream&,const std::string&);
			void createClassAttributes(const apidb::symbols::Table&,std::ofstream&);
			void createClassPrivate(std::ofstream&);
			//void createClassAttributesCPP(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublic(std::ofstream&);
			void writeKeyValue(const apidb::symbols::Table&,std::ofstream&);
			bool createDatconnect(std::ofstream& file,bool log);
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            //void getKeyJava(std::ofstream& ofile, const symbols::Symbol* k);
            
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            //void getKey2Java(std::ofstream& ofile, const symbols::Symbol* k);
            /**
            * \private
            * */
			//apidb::Analyzer& analyzer;
			//std::ofstream* writeResults;//erreglo de writeoutput files
			//std::string projectH;
			//std::string projectCPP;
		};

		/**
		* \private No es parte del API
		* */
		class CMake : public Generator
		{
		public:
			virtual bool generate(bool log);
			virtual ~CMake();
			CMake(apidb::Analyzer&,const ConfigureProject&);

		private:
			//apidb::Analyzer& analyzer;
			std::ofstream cmakelists;
			std::ofstream toolkitcommonconifg;
			std::ofstream toolkitclientdbConfig;
			std::ofstream config;
			std::ofstream developing;
			//Options options;
			//const ConfigureProject& configureProject;
		};

		/**
		* \private No es parte del API
		* */
		class Maven : public Generator
		{
		public:
			virtual bool generate(bool log);
			virtual ~Maven();
			Maven(apidb::Analyzer&,const ConfigureProject&);
            
		private:
			std::string filename;
		};
        
        
		class PHP : public Generator
		{
		public:
            /**
             * \brief Llmada directameten por el driver para inila contruccion
             * */
			virtual bool generate(bool log);
			PHP(Analyzer&,const ConfigureProject&);
			const std::string& getHeaderName() const;
			virtual ~PHP();
            
		private:
            //contructor
			void writeDefaultContructor(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyRawDataContructor(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructor(const apidb::symbols::Table&,std::ofstream&);
            
            //selects
			void writeSelects(const apidb::symbols::Table&,std::ofstream&);
            //void writeSelectInstancetObjectData(const apidb::symbols::Table&,std::ofstream&);
            
            //downloads
			void writeDownloads(const apidb::symbols::Table&,std::ofstream&);
            
            //getter
            void writeGetters(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //updatter
            void writeUppdaters(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //inserts
			void writeInsert(const apidb::symbols::Table&,std::ofstream&);            
            
            //methodes
			void createClassMethodes(const apidb::symbols::Table&,std::ofstream&);            
            
            //updatter
            void writeRemoves(const apidb::symbols::Table& table, std::ofstream& ofile);
            
			//varias
			bool create(std::ofstream& file, bool log, const symbols::SymbolsTable&);
			bool create(std::ofstream& file, bool log, const symbols::ISpace*);
            
            //
            void createClass(const apidb::symbols::Table&,std::ofstream&,const std::string&);
			void createClassAttributes(const apidb::symbols::Table&,std::ofstream&);
			void createClassPrivate(std::ofstream&);
			//void createClassAttributesCPP(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublic(std::ofstream&);
			void writeKeyValue(const apidb::symbols::Table&,std::ofstream&);
			bool createDatconnect(std::ofstream& file,bool log);
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            virtual void getKey(std::ofstream& ofile, const symbols::Symbol* k);
            
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            virtual void getInheritKey(std::ofstream& ofile, const symbols::Symbol* k);
            
            /**
            * @brief genera una llada recursiva para el componete indicado
            * @condition se debe llamar con k->symbolReferenced
            * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
            */
            //void getKey2Java(std::ofstream& ofile, const symbols::Symbol* k);
            /**
            * \private
            * */
			//apidb::Analyzer& analyzer;
			//std::ofstream* writeResults;//erreglo de writeoutput files
			//std::string projectH;
			//std::string projectCPP;
		};
	}
}
}
#endif
