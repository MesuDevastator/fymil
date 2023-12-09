/*
 * Copyright 2023 Mesu Devastator
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <exception>
#include <memory>

#include <log4cxx/logmanager.h>
#include <log4cxx/propertyconfigurator.h>

#include <xercesc/util/PlatformUtils.hpp>

#include "fymil/context.hpp"
#include "fymil/program.hpp"
#include "fymil/element_manager.hpp"
#include "fymil/elements/print.hpp"
#include "fymil/elements/string.hpp"
#include "fymil/utilities/transcode.hpp"

int main(const int argc, const char* argv[])
{
	log4cxx::PropertyConfigurator::configure("log4cxx.properties");
	log4cxx::LoggerPtr lg{ log4cxx::LogManager::getRootLogger() };

	if (argc < 2)
	{
		LOG4CXX_FATAL(lg, "Executable unspecified");
		return 1;
	}

	try
	{
		xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& e)
	{
		LOG4CXX_FATAL(lg, "Failed to initialize Xerces library: " << fymil::utilities::transcode(e.getMessage()).get());
		return 1;
	}
	LOG4CXX_TRACE(lg, "Initialized Xerces library");

	{
		LOG4CXX_TRACE(lg, "Registering internal commands");
		fymil::element_manager element_manager{};
		try
		{
			element_manager.register_element("print", [](xercesc::DOMElement* element)
			{ return reinterpret_cast<fymil::elements::element*>(new fymil::elements::print{ element }); });
			element_manager.register_element("s", [](xercesc::DOMElement* element)
			{ return reinterpret_cast<fymil::elements::element*>(new fymil::elements::string{ element }); });
		}
		catch (const std::exception& e)
		{
			LOG4CXX_FATAL(lg, "Unexpected exception occurred while registering internal elements: " << e.what());
			return 1;
		}
		catch (...)
		{
			LOG4CXX_FATAL(lg, "Unexpected exception occurred while registering internal elements");
			return 1;
		}

		std::unique_ptr<fymil::program> executable;
		std::unique_ptr<fymil::context> context{ std::make_unique<fymil::context>() };

		try
		{
			executable = std::make_unique<fymil::program>(argv[1], element_manager);
		}
		catch (const std::exception& e)
		{
			LOG4CXX_FATAL(lg, "Unexpected exception occurred while parsing executable: " << e.what());
			return 1;
		}
		catch (...)
		{
			LOG4CXX_FATAL(lg, "Unexpected exception occurred while parsing executable");
			return 1;
		}

		try
		{
			executable->execute(*context,element_manager);
		}
		catch (const std::exception& e)
		{
			LOG4CXX_FATAL(lg, "Unexpected exception occurred while executing executable: " << e.what());
			return 1;
		}
		catch (...)
		{
			LOG4CXX_FATAL(lg, "Unexpected exception occurred while executing executable");
			return 1;
		}
	}

	xercesc::XMLPlatformUtils::Terminate();
	LOG4CXX_TRACE(lg, "Terminated Xerces library");

	log4cxx::LogManager::shutdown();

	return 0;
}
