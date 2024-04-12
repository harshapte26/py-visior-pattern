#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Base class for build targets
class BuildTarget {
public:
    virtual void accept(class BuildVisitor& visitor) = 0;
    virtual ~BuildTarget() {}
};

// Concrete class for cc_binary build target
class CCBinary : public BuildTarget {
public:
    string type;
    string name;
    vector<string> deps;
    vector<string> srcs;

    CCBinary(const string& _type, const string& _name, const std::vector<std::string>& _deps, const std::vector<std::string>& _srcs)
           : type(_type), name(_name), deps(_deps), srcs(_srcs) {}

    void accept(BuildVisitor& visitor) override;
};

// Concrete class for cc_library build target
class CCLibrary : public BuildTarget {
public:
    string type;
    string name;
    vector<string> srcs;
    vector<string> hdrs;
    vector<string> deps;

    CCLibrary(const std::string& _type, const std::string& _name, const std::vector<std::string>& _srcs, const std::vector<std::string>& _hdrs, const std::vector<std::string>& _deps)
            : type(_type), name(_name), srcs(_srcs), hdrs(_hdrs), deps(_deps)  {}

    void accept(BuildVisitor& visitor) override;
};

// Visitor interface
class BuildVisitor {
public:
    virtual void visit(CCBinary& binary) = 0;
    virtual void visit(CCLibrary& library) = 0;
    virtual ~BuildVisitor() {}
};

// Concrete visitor class for generating .ninja file
class NinjaGenerator : public BuildVisitor {
private:
    std::ofstream outFile; // Output file stream
public:
    NinjaGenerator(const std::string& filename) {
        outFile.open(filename); // Open the output file
        // Add the static lines at the beginning of the file:
        outFile << "rule compile\n";
        outFile << "  command = g++ -c $in -o $out\n";
        outFile << "\n";
        outFile << "rule archive\n";
        outFile << "  command = ar rcs $out $in\n";
        outFile << "\n";
        outFile << "rule link\n";
        outFile << "  command = g++ $in -o $out\n";
    }

    ~NinjaGenerator() {
        outFile.close(); // Close the output file
    }

    void visit(CCBinary& binary) override {
        outFile << "build " << binary.name << ": link ";

        // Iterate over each binary source and output it
        for (const auto& src : binary.srcs) {
            outFile << src << " ";
        }

        for (const auto& dep : binary.deps) {
            outFile << dep << ".a ";
        }
        outFile << "\n";
    }

    void visit(CCLibrary& library) override {
        // std::cout << "build " << library.name << ".o: compile ";
        outFile << "build " << library.name << ".o: compile ";
        for (const auto& src : library.srcs) {
            // std::cout << src << " ";
            outFile << src << " ";
        }
        // std::cout << "\n";

        // std::cout << "build " << library.name << ".a: archive " << library.name << ".o\n";
        outFile << "\n";

        outFile << "build " << library.name << ".a: archive " << library.name << ".o\n";
    }
};

void CCBinary::accept(BuildVisitor& visitor) {
    visitor.visit(*this);
}

void CCLibrary::accept(BuildVisitor& visitor) {
    visitor.visit(*this);
}

vector<unique_ptr<BuildTarget>> targets;

void build(vector<unique_ptr<BuildTarget>> &targetss);

template<typename T>
void target(T &&t) {
    targets.push_back(std::make_unique<T>(t));
}
