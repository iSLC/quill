/**
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/detail/misc/Utilities.h"
#include "quill/handlers/Handler.h"
#include <memory>
#include <string>
#include <vector>

namespace quill
{
namespace detail
{
/**
 * The logger object is broken down to LoggerDetails and Logger as we end up in circular include
 * references if we include both in LogRecord
 *
 * Logger includes LogRecord as it needs it to create it, and LogRecord needs to read the
 * LoggerDetails later during the backend thread processing, but we don't want to include Logger
 */
class LoggerDetails
{
public:
  /**
   * Constructor
   * @param name
   */
  LoggerDetails(char const* name, Handler* handler)
  {
    safe_strncpy(_name, name);
    _handlers.push_back(handler);
  }

  /**
   * Constructor
   * @param name
   */
  LoggerDetails(char const* name, std::vector<Handler*> handlers)
  {
    safe_strncpy(_name, name);
    _handlers = std::move(handlers);
  }

  /**
   * Deleted
   */
  LoggerDetails(LoggerDetails const&) = delete;
  LoggerDetails& operator=(LoggerDetails const&) = delete;

  /**
   * Destructor
   */
  ~LoggerDetails() = default;

  /**
   * @return The name of the logger
   */
  QUILL_NODISCARD char const* name() const noexcept { return _name.data(); }

  /**
   * @return a vector of all handlers of this logger, called by the backend worker thread
   */
  QUILL_NODISCARD std::vector<Handler*> const& handlers() const noexcept { return _handlers; }

private:
  std::array<char, 24> _name; /** Because size of string in gcc is 32 we use an array here as we need the Logger object to fit all in a single cache line */
  std::vector<Handler*> _handlers;
};
} // namespace detail
} // namespace quill