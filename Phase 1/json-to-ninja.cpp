#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "json.hpp"

using std::ifstream;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;
using nlohmann::json;

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
        // std::cout << "build " << binary.name << ": link " << binary.srcs << " ";
        outFile << "build " << binary.name << ": link " << binary.srcs << " ";
        for (const auto& dep : binary.deps) {
            // std::cout << dep << ".a ";
            outFile << dep << ".a ";
        }
        // std::cout << "\n";
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

int main() {
    // Read JSON file
    ifstream file("foo.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open JSON file." << endl;
        return 1;
    }

    // Parse JSON
    json json;
    try {
        file >> json;
    } catch (const json::parse_error& e) {
        cerr << "Parse error: " << e.what() << endl;
        return 1;
    }

    // Process JSON data and create corresponding C++ objects
    vector<unique_ptr<BuildTarget>> targets;
    for (const auto& item : json) {
        string type = item["type"];
        if (type == "cc_binary") {
            auto binary = std::make_unique<CCBinary>();
            binary->name = item["name"];
            binary->srcs = item["srcs"];
            // Handle dependencies
            for (const auto& dep : item["deps"]) {
                binary->deps.push_back(dep.get<string>());
            }
            targets.push_back(std::move(binary));
        } else if (type == "cc_library") {
            auto library = std::make_unique<CCLibrary>();
            library->name = item["name"];
            // Handle srcs
            for (const auto& src : item["srcs"]) {
                library->srcs.push_back(src.get<string>());
            }
            // Handle hdrs
            for (const auto& hdr : item["hdrs"]) {
                library->hdrs.push_back(hdr.get<string>());
            }
            // Handle deps
            if (item.find("deps") != item.end()) {
                library->deps = item["deps"].get<string>();
            }
            targets.push_back(std::move(library));
        }
        // Add more conditions for other types if necessary
    }

    // Generate .ninja file
    NinjaGenerator generator("generated_build.ninja");
    for (const auto& target : targets) {
        target->accept(generator);
    }

    return 0;
}
