#include "bets42/deepthgt/AdminHandler.hpp"
#include "bets42/earth/Exception.hpp"
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include <glog/log_severity.h>
#include <cassert>
#include <iostream>
#include <string>

namespace prog_opts = boost::program_options;

namespace {

    struct LogInitializer
    {
        LogInitializer(const char* programName, google::LogSeverity logLevel) 
        {
            google::InitGoogleLogging(programName);
            google::LogToStderr();
            FLAGS_minloglevel = logLevel;
        }

        ~LogInitializer() {
            google::ShutdownGoogleLogging();
        }
    };

} //namespace annonymous

int main(int argc, char *argv[])
{
    const LogInitializer logInititializer(argv[0], google::INFO);

    LOG(INFO) << "Starting 42bets engine";

    try
    {
        //define program options
        prog_opts::options_description general("General options");
        general.add_options()
            ("help", "Show help information");

        prog_opts::options_description running("Running options");
        running.add_options()
            ("algo", prog_opts::value<std::string>(), "Name of the earth algo to run; choose from [PRE_RACE_HORSE]")
            ("exchanges", prog_opts::value< std::vector<std::string> >(), "Name(s) of the exchanges which will be available for use; choose from [BETFAIR|BETDAQ]")
            ("admin_port", prog_opts::value<unsigned>(), "Admin port on which admin requests will be listened for")
            ("log_dir", prog_opts::value<std::string>(), "Log directory into which log files will be written");

        prog_opts::options_description all("42bets Usage");
        all.add(general).add(running);

        //process and store command line args
        prog_opts::variables_map args;
        try
        {
            prog_opts::store(prog_opts::parse_command_line(argc, argv, all), args);
            prog_opts::notify(args);
        }
        catch(const prog_opts::error& e)
        {
            LOG(FATAL) << "Failed start 42bets with given command line argument(s) " 
                       << std::vector<std::string>(argv+1, argv + argc) << ": " 
                       << e.what();
        }

        //run algo
        if(args.count("algo") 
            && args.count("exchanges") 
            && args.count("admin_port")
            && args.count("log_dir"))
        {
            const auto algo(args["algo"].as<std::string>());
            const auto exchanges(args["exchanges"].as< std::vector<std::string> >());
            const auto adminPort(args["admin_port"].as<unsigned short>());
            const auto logDir(args["log_dir"].as<std::string>());

            LOG(INFO)   << "Preparing to run;"
                        << " algo=" << algo 
                        << ", exchanges=" << exchanges 
                        << ", adminPort=" << adminPort 
                        << ", logDir=" << logDir;

            bets42::deepthgt::AdminHandler adminHandler(adminPort);
                 /*
            std::set<bets42::Exchange> exchanges;
            for(const auto exchange : args["exchanges"].as< std::vector<std::string> >())
            {
                const auto result = exchanges.insert(bets42::createExchange(exchange, adminHandler));
                if(!result.second)
                {
                    //throw earth::Exception(__FILE__, __LINE__)
                    //    << "Can't create " << exchange << " exchange multiple times";
                }
            }

            bets42::Client client(bets42::createClient(algo, exchanges, adminHandler));

            //start client on it's own thread
            client.start();
                           */

        }
        //get help text
        else if(args.count("help"))
        {
            LOG(INFO) << "Requested help information";
            std::cout << all << std::endl;
        }
        else
        {
            LOG(FATAL) << "Invalid command line argument(s) provided " 
                       << std::vector<std::string>(argv+1, argv + argc) 
                       << ", see --help"; 
        }
    }
    catch(const bets42::earth::Exception& e)
    {
        LOG(FATAL) << "Caught bets42::Exception in main(); exception: " << e.what();
    }
    catch(const std::exception& e)
    {
        LOG(FATAL) << "Caught std::exception in main(); exception: " << e.what();
    }
    catch(...)
    {
        LOG(FATAL) << "Caught unknown exception in main()";
    }

    return 0;
}
