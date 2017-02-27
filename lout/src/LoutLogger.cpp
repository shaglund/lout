// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "lout/LoutLogger.h"
#include "lout/StringItem.h"
#include "lout/LevelItem.h"


namespace lout {

LoutLogger::~LoutLogger()
{
	Flush();
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(const loglevel::ILogLevel& level)
{
	myItems.push_back( std::make_shared<LevelItem>( level ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(const std::string& msg)
{
	myItems.push_back( std::make_shared<StringItem>( msg ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(int8_t value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(int16_t value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(int32_t value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(int64_t value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(uint8_t value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(uint16_t value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(uint32_t value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;

}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(uint64_t value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(double value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(long double value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(float value)
{
	myItems.push_back( std::make_shared<StringItem>( std::to_string( value ) ) );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<( std::shared_ptr<LogItem> item )
{
	myItems.push_back( item );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
LoutLogger& LoutLogger::operator<<(const lout::Flush&)
{
	Flush();
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void LoutLogger::AppendMsg(const std::string& msg)
{
	myCurrentMessage << msg;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void LoutLogger::SetLevel(const loglevel::ILogLevel& level)
{
	myCurrentLevel = level;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void LoutLogger::Flush()
{
	for( auto item : myItems )
	{
		item->Log( *this );
	}

	myItems.clear();

	std::string msg = myCurrentMessage.str();

	if( !msg.empty() )
	{
		if( myCategory.empty() )
		{
			Lout::Get().Log( myCurrentLevel, msg );
		}
		else
		{
			Lout::Get().LogWithCategory( myCurrentLevel, myCategory, msg );
		}
	}

	myCurrentMessage.str( "" );
	myCurrentMessage.clear();
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void LevelItem::Log(LoutLogger& l)
{
	l.SetLevel( myLevel );
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void StringItem::Log(LoutLogger& l)
{
	l.AppendMsg( myMsg );
}


}

