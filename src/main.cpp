#include <iostream>
#include <string>

#include "app/Application.h"
#include "app/CommandLineParser.h"
#include "app/HelpPrinter.h"
#include "common/Exceptions.h"

int main(int argc, char** argv) {
    try {

        if (argc == 1) {
            HelpPrinter::Print(std::cout);
            return 0;
        }

        const std::string first_arg = argv[1];
        if (first_arg == "-h" || first_arg == "--help") {
            HelpPrinter::Print(std::cout);
            return 0;
        }

        const Config config = CommandLineParser::Parse(argc, argv);
        Application app;
        app.Run(config);
        return 0;
    } catch (const ImageProcessorError& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {

        std::cerr << "Unexpected error: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unexpected unknown error\n";
        return 1;
    }
}
