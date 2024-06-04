/**
 * \file .
 * \brief Interface to JSON serializer.
 */
#pragma once
#include <string>

/**
 * Serializes an object to a JSON formatted string.
 *
 * @tparam T a serializable structure or container
 * @param object data to be serialized
 * @returns string JSON formatted
 */
template <class T>
std::string toJsonString(const T &object);
