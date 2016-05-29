//
// Created by Per Malmberg on 2016-05-21.
//

#include <iostream>
#include "output/StdOutput.h"


namespace lout {
namespace output {

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
StdOutPrinter::StdOutPrinter() :
		IOutput( &std::cerr )
{ }

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
StdOutPrinter::StdOutPrinter( std::ostream* fallbackStream ) :
		IOutput( fallbackStream )
{ }

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void StdOutPrinter::LogActual(const loglevel::ILogLevel& level, const std::string& msg)
{
	std::cout << "[" << level << "]" << msg << std::endl;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void StdOutPrinter::LogWithCategoryActual(const loglevel::ILogLevel& level, const std::string& category,
                                          const std::string& msg){
	std::cout << "[" << level << "/" << category << "]" << msg << std::endl;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void StdOutPrinter::Flush() noexcept
{
	std::cout.flush();
}

}
}