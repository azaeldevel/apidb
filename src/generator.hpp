namespace apidb
{
	class Generator
	{
	public:
		enum OutputLenguajes
		{
			C,
			CPP,
			Java,
			CSHARP,
			Perl,
			Python
		};
		
		virtual bool generate(apidb::mysql::Driver& driver) = 0;
	};
}
