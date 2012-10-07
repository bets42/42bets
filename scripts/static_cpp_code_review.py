#!/usr/bin/env python

"""Statically analyses C++ sources using google-lint, Artistic Style and cppcheck

The purpose of this script it to identify areas of the code that *may* not incorrect 
or not be compliant with the projects style guidelines. It uses three open source 
projects to both highlight violations through a warning as well as amend files for a 
particular violations.

The scripts are applied in the following order:

    1. Artistic Style - corrects formatting including bracket style, padding and 
    identation
    2. cppcheck - performs static analysis for a range of things including memory 
    leaks, unused variables/functions, uninitialised variables and bounds checks.
    3. google-lint - checks the source files against the Google Style Guide; some 
    style guidelines are explicitly ignored.

For more information on each see their respective online documentation."""

import fnmatch
import optparse
import os
import subprocess
import sys

def print_header(text):
    """Print stylised header text for delimiting results"""
    print '\n' + ('*' * 40) + '\n' + text + " Results\n" + ('*' * 40)

def print_analysis(cmd):
    """Execute the static analysis with the given cmd, returning the output"""
    popen = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    popen.wait()
    print popen.stdout.read()

def main():
    #define command line arguments
    parser = optparse.OptionParser("Usage: %prog [options]")
    parser.add_option("--astyle", dest="astyle", help="Artistic Style binary", metavar="FILE")
    parser.add_option("--cppcheck", dest="cppcheck", help="cppcheck binary", metavar="FILE")
    parser.add_option("--cpplint", dest="cpplint", help="google-lint script", metavar="FILE")
    parser.add_option("--root", dest="root", help="Absolute path to source root directory", type="string")
    (options, args) = parser.parse_args()

    if options.root:

        #build a list of files to analyse
        files = []
        for root, dirnames, filenames in os.walk(options.root):
            for filename in (fnmatch.filter(filenames, ('*.hpp')) + fnmatch.filter(filenames, ('*.cpp'))):
                files.append(os.path.join(root, filename))

        print_header("Files to analyse")
        print '\n'.join(f for f in files)

        if options.astyle:
            astyle_cmd = [
                options.astyle,
                "--add-brackets",
                "--align-pointer=type",
                "--align-reference=type",
                "--break-blocks",
                "--indent-classes",
                "--indent-col1-comments",
                "--indent-switches",
                "--indent-namespaces",
                "--indent=tab",
                "--min-conditional-indent=0",
                "--pad-oper",
                "--style=allman",
                "--unpad-paren",
                "--verbose"
            ]

            print_header("Artistic Style")
            print_analysis(astyle_cmd + files)

        if options.cppcheck:
            cppcheck_cmd = [
                options.cppcheck,
                "-I " + options.root,
                "--enable=all",
                "--inconclusive",
                "--std=c11",
                "--std=c++11",
                "--verbose"
            ]

            print_header("cppcheck")
            print_analysis(cppcheck_cmd + files)

        if options.cpplint:
            cpplint_cmd = [
                "python",
                options.cpplint,
                """--filter=
                    -build/namespaces,
                    -legal/copyright,
                    -readability/streams,
                    -readability/todo,
                    -runtime/int,
                    -whitespace/braces,
                    -whitespace/labels,
                    -whitespace/line_length,
                    -whitespace/newline,
                    -whitespace/tab""", 
                "--counting=detailed"
            ]

            print_header("google-lint")
            print_analysis(cpplint_cmd + files)

    else:
        parser.error("Incorrect number/set of arguments provided, see --help")

if __name__ == "__main__":
    main()

