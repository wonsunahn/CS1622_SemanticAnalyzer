/** Code by @author Wonsun Ahn, Fall 2025
 *
 * The parser test driver file.
 */

// stdlib headers
#include <string>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

extern "C"
{
#include "proj2.h"
#include "y.tab.h"
}
#include "proj3.h"

/* input for yylex() */
extern FILE *yyin;
/* printTreeText file pointer */
extern FILE *treelst;
/* printTreeGraphviz file pointer */
extern FILE *treeimg;
/* Root of the syntax tree */
extern tree SyntaxTree;
/* Print syntax tree in text format */
void printTreeText(tree nd, int depth);
/* Print syntax tree in graphviz format */
void printTreeGraphviz(tree nd, std::string parentNodeId, std::string currentNodeId);
/* Make symbol table function */
void MkST(tree);

void printUsage()
{
  printf("USAGE: parser [OPTIONS] <source file path>\n");
  printf("Builds syntax tree and symbol table out of MINI-JAVA source code.\n\n");
  printf("  -h           this help screen.\n");
  printf("  -p <file>    output graphviz file for syntax tree.\n");
  printf("  -v           verbose output (displays symbol table and parse tree).\n");
  printf("  -w           display semantic errors as warnings.\n");
  printf("  -l           prints line numbers for each symbol in symbol table.\n");
}

int main(int argc, char **argv)
{
  std::string outputFileName, inputFileName, graphVizFileName;
  bool verbose = false;
  bool printLineNo = false;
  char c;

  while ((c = getopt(argc, argv, "hp:vwl0")) != -1)
  {
    switch (c)
    {
    case 'h':
      printUsage();
      return 0;
    case 'p':
      graphVizFileName = optarg;
      treeimg = fopen(graphVizFileName.c_str(), "w");
      if (treeimg == NULL)
      {
        fprintf(stderr, "Cannot open %s for writing.\n",
                graphVizFileName.c_str());
        exit(1);
      }
      break;
    case 'v':
      verbose = true;
      break;
    case 'w':
      printSemanticErrors = true;
      break;
    case 'l':
      printLineNo = true;
      break;
    case '?':
      if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr,
                "Unknown option character `\\x%x'.\n",
                optopt);
      printUsage();
      return 0;
    default:
      abort();
    }
  }
  if (optind != argc - 1)
  {
    printUsage();
    return 0;
  }

  inputFileName = argv[optind];
  FILE *inputFile = fopen(inputFileName.c_str(), "r");
  if (inputFile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading.\n",
            inputFileName.c_str());
    exit(1);
  }

  /* Make syntax tree */
  SyntaxTree = NULL;
  yyin = inputFile;
  yyparse();

  fclose(inputFile);

  if (SyntaxTree == NULL)
  {
    fprintf(stderr, "Syntax Tree not created, exiting...\n");
    exit(1);
  }
  
  /* Make symbol table */
  MkST(SyntaxTree);

  /* If verbose, print out syntax tree and symbol table. */
  if (verbose)
  {
    STPrint(stdout, printLineNo);
    treelst = stdout;
    printTreeText(SyntaxTree, 0);
  }

  /* Print out syntax tree to graphviz file if requested. */
  if (treeimg != NULL)
    printTreeGraphviz(SyntaxTree, "", "n");

  return 0;
}