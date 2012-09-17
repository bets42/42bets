#include "bets42/arthur/exception.hpp"
#include "bets42/deepthgt/TradingEngine.hpp"
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include <glog/log_severity.h>
#include <cassert>
#include <iostream>
#include <string>

namespace prog_opts = boost::program_options;

namespace {

    struct MainException : public bets42::arthur::streaming_exception<MainException>
    {
        MainException(const std::string& file, const long line)
            : bets42::arthur::streaming_exception<MainException>(file, line) {}
    };  

    struct LogInitializer
    {
        LogInitializer(const std::string& dir,   
                       const std::string& filename)
        {
            google::InitGoogleLogging(filename.c_str());
            google::SetLogDestination(google::INFO, dir.c_str());
            google::SetLogSymlink(google::INFO, filename.c_str());
            google::LogToStderr();

            FLAGS_minloglevel = google::INFO;

            LOG(INFO) << "Started logging at level " << google::GetLogSeverityName(FLAGS_minloglevel);  
        }

        ~LogInitializer() 
        {
            LOG(INFO) << "Finished logging";  
            google::ShutdownGoogleLogging();
        }
    };
       
} //namespace annonymous

//TODO: make log level settable via cmd line arguments

int main(int argc, char *argv[])
{
    try
    {
        //define command line arguments
        prog_opts::options_description general("General options");
        general.add_options()
            ("help", "Show help information");

        prog_opts::options_description running("Running options");
        running.add_options()
            ("algo", prog_opts::value<std::string>(), "Algo which trading engine should run; select from [PRE_RACE_HORSE_SWING]")
            ("exchanges", prog_opts::value<std::vector<std::string>>(), "Exchange(s) to which the trading engine will connect; select from [BETFAIR|BETDAQ]")
            ("instance", prog_opts::value<unsigned>(), "Instance number, useful when running multiple instances of same algo")
            ("cmd_port", prog_opts::value<unsigned short>(), "Port on which commands will be listened for")
            ("log_dir", prog_opts::value<std::string>(), "Log directory into which log files will be written")
            ("log_name", prog_opts::value<std::string>(), "Name of the file to write logs to");

        prog_opts::options_description all("42bets Usage");
        all.add(general).add(running);

        //parse and store cmd line arguments
        prog_opts::variables_map varsMap;
        try
        {
            prog_opts::store(prog_opts::parse_command_line(argc, argv, all), varsMap);
            prog_opts::notify(varsMap);
        }
        catch(const prog_opts::error& e)
        {
            throw MainException(__FILE__, __LINE__) 
                << "Failed to start 42bets with given command line argument(s) " 
                << std::vector<std::string>(argv+1, argv + argc) << "; exception=" 
                << e.what();
        }

        
        //act on cmd line arguments
        if(varsMap.count("help"))
        {
            std::cout << all << std::endl;
        }
        else if(varsMap.count("algo")
                && varsMap.count("exchanges")
                && varsMap.count("instance")
                && varsMap.count("cmd_port") 
                && varsMap.count("log_dir") 
                && varsMap.count("log_name"))
        {
            //logging
            const auto logDir(varsMap["log_dir"].as<std::string>());
            const auto logName(varsMap["log_name"].as<std::string>() 
                                + boost::lexical_cast<std::string>(varsMap["instance"].as<unsigned>()));

            const LogInitializer logInit(logDir, logName);

            //create & run engine
            try
            {   
                const auto algo(varsMap["algo"].as<std::string>());
                const auto exchanges(varsMap["exchanges"].as<std::vector<std::string>>());
                const auto cmdPort(varsMap["cmd_port"].as<unsigned short>());

                auto engine(bets42::deepthgt::TradingEngineFactory::create(algo, exchanges, cmdPort));
                engine->run();
            }
            catch(const std::exception& e)
            {
                throw MainException(__FILE__, __LINE__)
                    << "Error whilst running trading engine; exception=" << e.what();
            }
        }
        else
        {
            throw MainException(__FILE__, __LINE__)
                << "Invalid command line argument(s) provided " 
                << std::vector<std::string>(argv+1, argv + argc) 
                << ", see --help"; 
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw MainException(__FILE__, __LINE__)
            << "Caught exception in main(); exception=" << e.what();
    }
    catch(...)
    {
        std::cerr << "unknown exception" << std::endl;
        throw MainException(__FILE__, __LINE__)
            << "Caught unknown exception main()";
    }

    return 0;
}
