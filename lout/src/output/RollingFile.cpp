//
// Created by Per Malmberg on 2016-05-25.
//

#include <iostream>
#include <experimental/filesystem>
#include <algorithm>
#include <util/Sizes.h>
#include "output/RollingFile.h"


namespace lout {
namespace output {

namespace fs = std::experimental::filesystem;

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
RollingFile::RollingFile(const std::string& pathToOutputFolder, std::unique_ptr<IRollingFileName> nameGiver,
                         util::Bytes maximumLogSize, int filesToKeep)
		: RollingFile( pathToOutputFolder, std::move( nameGiver ), maximumLogSize, filesToKeep, &std::cerr )
{

}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
RollingFile::RollingFile(const std::string& pathToOutputFolder, std::unique_ptr<IRollingFileName> nameGiver,
                         util::Bytes maximumLogSize, int filesToKeep, std::ostream* fallbackStream)
		: IOutput( fallbackStream ), myPathToOutputFolder( pathToOutputFolder ), myOutput(),
		  myNameGiver( std::move( nameGiver ) ),
		  myMaximumLogSize( maximumLogSize ),
		  myFilesToKeep( filesToKeep )
{

}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
RollingFile::~RollingFile()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
size_t RollingFile::GetCurrentLogCount() const
{
	std::vector<std::string> currentLogs;
	GetCurrentLogFiles( currentLogs );
	return currentLogs.size();
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void RollingFile::GetCurrentLogFiles( std::vector<std::string> &target ) const
{
	try {
		fs::directory_iterator dir( myPathToOutputFolder );

		for( auto & entry : dir ) {
			if( fs::is_regular_file( entry.path() ) ) {
				const fs::path& p = entry.path();
				const std::string fileName = p.filename().string();
				if( myNameGiver->Matches( fileName ) ) {
					target.push_back( p.string() );
				}
			}
		}
	}
	catch( ... )
	{

	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void RollingFile::Flush() noexcept
{
	if( myOutput )
	{
		myOutput->Flush();
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void RollingFile::LogActual(const loglevel::ILogLevel& level, const std::string& msg)
{
	Open();

	if( myOutput )
	{
		myOutput->Log( level, msg );
	}

	Roll();
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void RollingFile::LogWithCategoryActual(const loglevel::ILogLevel& level, const std::string& category,
                                        const std::string& msg)
{
	Open();

	if( myOutput )
	{
		myOutput->LogWithCategory( level, category, msg );
	}

	Roll();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void RollingFile::Roll()
{
	if( myOutput && myOutput->GetCurrentSize() > myMaximumLogSize.GetBytes() )
	{
		Close();
		CleanOldFiles();
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void RollingFile::Open()
{
	if( !myOutput )
	{
		fs::path file( myPathToOutputFolder );
		file /= myNameGiver->GetNextName();

		myOutput = std::make_unique<FileOutput>( file.string() );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void RollingFile::Close()
{
	if( myOutput )
	{
		myOutput->Close();
		myOutput.reset( nullptr );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
//std::string RollingFile::PathCombine(const std::string& p1, const std::string& p2)
//{
//	std::string path = p1;
//
//	if( path.length() > 0 )
//	{
//		if( path.at( p1.length() - 1 ) != fs::path::preferred_separator )
//		{
//			path += pathSeparator;
//		}
//	}
//
//	path += p2;
//
//	return path;
//}



class AgeSorter
{
public:
	bool operator()( fs::path a, fs::path b )
	{
		auto timeA = fs::last_write_time( a );
		auto timeB = fs::last_write_time( b );
		return timeA < timeB;
	}
};

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void RollingFile::CleanOldFiles() const
{
	try 
	{
		fs::directory_iterator dir( myPathToOutputFolder );

		std::vector<std::string> allFiles;
		GetCurrentLogFiles( allFiles );

		// Sort in decending age order
		std::sort( allFiles.begin(), allFiles.end(), AgeSorter() );
		// Remove from back until we have removed as many as we want to save
		if(static_cast<int>( allFiles.size() ) > myFilesToKeep )
		{
			// Remove newest files from the list - we're keeping those.
			for( int i = 0; i < myFilesToKeep; ++i ) 
			{
				allFiles.pop_back();
			}

			// No delete the files remaining in our list
			for( auto fullPath : allFiles ) 
			{
				try 
				{
					fs::remove( fs::path( fullPath ) );
				}
				catch( ... )
				{
					// If we can't delete it, there is nothing we can do.
				}
			}
		}
	}
	catch( ... ) 
	{

	}
}

}
}