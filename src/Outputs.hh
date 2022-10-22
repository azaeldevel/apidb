
#ifndef OCTETOS_APIDB_OUTPUTS_HH
#define OCTETOS_APIDB_OUTPUTS_HH

namespace oct::api
{

class Output
{
public:
        Output(const Input&);
        virtual void write(std::ofstream&) = 0;
protected:
        const Input& in;
};
class OutputAPIAcces : public Output
{
public:
        OutputAPIAcces(const Input&);
        virtual void write(std::ofstream&);
protected:
};
class OutputGtkmm : public Output
{
public:
        OutputGtkmm(const Input&);
        virtual void write(std::ofstream&);
protected:
};


}

#endif