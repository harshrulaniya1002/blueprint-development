#include<bits/stdc++.h>
using namespace std;

//use cases
// 1) 3rd party vendor or 3rd party libraries
// 2) Legacy code (old code)
// 3) Different data format (XML to JSON, CSV to JSON etc)
// 4) Different communication protocols (SOAP to REST etc)
// 5) Different interfaces (old interface to new interface)
// 6) Reuse existing code with incompatible interfaces
// 7) Simplify complex interfaces
// 8) Integrate with external systems



class IReports{
    public:
    // Function to generate JSON report (virtual function)
    virtual string generateJSONReport(string &data)=0;

    // destructor
    virtual ~IReports(){};
};

//Adaptee: Provides XML data from a raw data
class XMLDataProvider{
    public:
    // Function to generate XML report
    // Expect data in "name:id" format
    string getXMLData(string &data){
        size_t sep = data.find(':');
        string name = data.substr(0, sep);
        string id = data.substr(sep+1);
        return "<report><name>"+name+"</name><id>"+id+"</id></report>";
    }
};



// Adapter : implements IReports and uses XMLDataProvider to get XML data
class ReportAdapter : public IReports {
    private:
    XMLDataProvider* xmlProvider;
    public:
    ReportAdapter(XMLDataProvider* provider){
        this->xmlProvider = provider;
    }

    string generateJSONReport(string &data) override{
        string xml = xmlProvider->getXMLData(data);

        size_t startName = xml.find("<name>") + 6;
        size_t endName = xml.find("</name>");
        string name = xml.substr(startName, endName - startName);

        size_t startId = xml.find("<id>") + 4;
        size_t endId = xml.find("</id>");
        string id = xml.substr(startId, endId - startId);

        return "{\"name\":\"" + name + "\",\"id\":\"" + id + "\"}";
    }
};


//Client Code works with IReports interface
class Client{
    public:
    void getReport(IReports* reportGen, string &data){
        string jsonReport = reportGen->generateJSONReport(data);
        cout << jsonReport << endl;
        cout << "Report Generated Successfully\n";
    }
};


int main()
{
  //create the adaptee
    XMLDataProvider* xmlProvider = new XMLDataProvider();

    //make our adapter
    IReports* reportAdapter = new ReportAdapter(xmlProvider);

    //client code
    Client* client = new Client();
    string data = "Alice:12345";

    client->getReport(reportAdapter, data);
}