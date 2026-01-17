#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <gtest/gtest.h>

/**
 * Reads the entire content of the specified file,
 * and returns it as a string.
 */
inline std::string read_file(const std::string &filepath)
{
  /**
   * Opens the file at the given path, and checks
   * if the file was opened successfully.
   */
  std::ifstream file_stream(filepath);
  if (!file_stream.is_open())
  {
    throw std::runtime_error("Could not open the specified file: " + filepath);
  }

  /**
   * Reads the content of the file into a string stream,
   * and returns a string by converting the string stream.
   */
  std::ostringstream file_buffer;
  file_buffer << file_stream.rdbuf();
  return file_buffer.str();
}

/**
 * Writes the given content to the specified file.
 */
inline void write_file(const std::string &filepath, const std::string &content)
{
  /**
   * Opens the file at the given path,
   * and writes the content to it.
   */
  std::ofstream file_stream(filepath);
  file_stream << content;
}

/**
 * Returns the snap directory path.
 */
inline std::string &get_snap_directory_path()
{
  static std::string snap_directory_path = "../../snap";
  return snap_directory_path;
}

/**
 * Ensures that the snaps directory is present or created,
 * before any snapshot files are read or written.
 */
inline void upsert_snap_directory()
{
  std::string snap_directory_path = get_snap_directory_path();
  if (!std::filesystem::exists(snap_directory_path))
  {
    std::filesystem::create_directory(snap_directory_path);
  }
}

/**
 * Asserts that the actual output matches the expected output.
 */
inline void validate_output(const std::string &test_id, const std::string &actual_output)
{
  /**
   * Makes sure that the snap directory exists throughout the process.
   */
  upsert_snap_directory();

  /**
   * Constructing the snap file path.
   */
  std::string test_name = "test_" + test_id;
  std::string snap_filepath = get_snap_directory_path() + "/" + test_name + ".snap";

  /**
   * Checking whether the snap does not exist yet.
   */
  if (!std::filesystem::exists(snap_filepath))
  {
    /**
     * Asking whether to create a new snap file,
     * or fail the test.
     */
    std::cout << "[WARN]: No snap found for the '" << test_name << "'." << std::endl;
    std::cout << "\n"
              << actual_output << "\n"
              << std::endl;
    std::cout << "Accept the output, and create a snap? (y/n)" << std::endl;

    /**
     * Getting the input from the user,
     * and creating the snap if accepted.
     */
    char response;
    std::cin >> response;
    if (response == 'y' || response == 'Y')
    {
      write_file(snap_filepath, actual_output);
      std::cout << "[INFO]: Snap created at '" << snap_filepath << "'." << std::endl;
      return;
    }
    else
    {
      FAIL() << ("Snap not accepted for the test: " + test_name);
    }
  }

  /**
   * Reading the expected output from the snap,
   * and failing the test if it does not match the actual output.
   */
  const std::string &expected_output = read_file(snap_filepath);
  if (actual_output != expected_output)
  {
    std::cout << "\n"
              << actual_output << "\n"
              << std::endl;
    FAIL() << "The snap does not match the expected output.";
  }
}