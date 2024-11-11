/*
 * PLEASE INSERT ADEQUATE CODE FOR: STAddPredefined()
 */

/*
The symbolTable is a list of all symbols in the program.  A symbol table entry is a map from attribute IDs to attribule values.  In addition to symbolTable, there is scopeStack that keeps track of current active scopes.  A scope is defined by the "bottom" index where the scope begins in the symbol table and the "top" index where the scope ends.
*/

// LLVM headers
#include <llvm/IR/Value.h>
#include <llvm/ADT/StringRef.h>

#include <vector>
#include <deque>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
extern "C"
{
#include "proj2.h"
}
#include "proj3.h"

/*
 * symbol table entry
 */
typedef struct
{
  std::unordered_map<int, uintptr_t> attrs; /* map from attribute names to values*/
} SymbolTableEntry;

/*
 * symbol table
 */
static std::vector<SymbolTableEntry> symbolTable;

/*
 * activation stack item
 */
typedef struct
{
  int bottom; /* the bottom symbol table index for this scope */
  int top;    /* the top symbol table index for this scope */
} Scope;

/*
 * activation stack where each stack item is a "scope" pointing to an offset in the symbol table where the scope begins
 */
static std::deque<Scope> scopeStack;

/*
 * current nesting level
 */
static int nesting = 0;

/*
 * whether to print out semantic errors
 */
bool printSemanticErrors = false;

/*
 * string table access functions defined in table.cpp.
 */
int getIndex(const char *string);
const char *getString(int i);

/************************ routines *****************************/

int GetSymbolTableSize()
{
  return symbolTable.size();
}

/*
 * STAddPredefined(): Add predefined names into symbol table.
 */
void STAddPredefined()
{
  int nStrInd, nSymInd; /* string table index */

  nStrInd = getIndex("system"); /* return string index of string "system" */
  if (nStrInd != -1)            /* "system" is stored in string table */
  {
    nSymInd = InsertEntry(nStrInd, CLASS, 0);
    SetAttr(nSymInd, PREDE_ATTR, true);
  }

  OpenBlock();

  nStrInd = getIndex("readln");
  if (nStrInd != -1)
  {
    nSymInd = InsertEntry(nStrInd, FUNC, 0);
    SetAttr(nSymInd, PREDE_ATTR, true);
    SetAttr(nSymInd, ARGNUM_ATTR, 1);
  }

  nStrInd = getIndex("println");
  if (nStrInd != -1)
  {
    nSymInd = InsertEntry(nStrInd, FUNC, 0);
    SetAttr(nSymInd, INIT_ATTR, 0);
    SetAttr(nSymInd, PREDE_ATTR, true);
    SetAttr(nSymInd, ARGNUM_ATTR, 1);
  }

  CloseBlock();
}

/*
 * InsertEntry():  builds a symbol table entry for id.  the current block is
 * searched for redeclaration error.  the id's name  and nesting level
 * attributes are set by this function.  writing ErrorMessage routine is your
 * work.
 */
int InsertEntry(int id, int kind, int lineno)
{
  /* id is already declared in the current block */
  if (LookUpHere(id) != -1)
  {
    return (-1);
  }
  SymbolTableEntry entry;
  entry.attrs[NAME_ATTR] = id;
  entry.attrs[KIND_ATTR] = kind;
  entry.attrs[LINENO_ATTR] = lineno;
  entry.attrs[NEST_ATTR] = nesting;
  entry.attrs[IS_USED_ATTR] = false;
  symbolTable.push_back(entry);

  assert(!scopeStack.empty());
  if (scopeStack.back().bottom == -1)
  {
    // If the current scope is empty, initialize both bottom and top.
    scopeStack.back().bottom = symbolTable.size() - 1;
    scopeStack.back().top = symbolTable.size() - 1;
  }
  else
  {
    // Otherwise, just add entry to current scope.
    scopeStack.back().top = symbolTable.size() - 1;
  }
  return symbolTable.size() - 1;
}

/*
 * LookUp(): search an id in the stack and return the symbol table entry
 * pointer, if found.  if the id is not in the stack, report error and push a
 * dummy item on the stack so that the same error will not be reported
 * repeatedly
 */
int LookUp(int id)
{
  for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
  {
    // If this scope is empty, continue to next scope.
    if (it->bottom == -1)
      continue;
    for (int idx = it->bottom; idx <= it->top; idx++)
      if (GetAttr(idx, NAME_ATTR) == id)
        return idx;
  }
  return -1;
}

/*
 * LookUpHere():  search an id in the stack for the current block.  It
 * returns the symbol table pointer if the id is found.  otherwise, return 0
 * this routine can be used to check if there is a forward declaration
 * for a procedure/function
 */
int LookUpHere(int id)
{
  // Check if the current scope is empty.
  if (scopeStack.empty() || scopeStack.back().bottom == -1)
    return -1;

  for (int idx = scopeStack.back().bottom; idx <= scopeStack.back().top; idx++)
  {
    if (GetAttr(idx, NAME_ATTR) == id)
      return idx;
  }

  return -1;
}

/*
 * LookUpField():  it is found unnecessary.  so it is not implemented
 */

/*
 * OpenBlock():  build a mark on the stack to indecate the beginning of a new
 * block. increment nesting level counter.  It is called when reserved words
 * "program", "procedure", "function" or "record" is encountered.  Note:
 * procedure or function name should be inserted into symbol table before
 * calling this routine.
 */
void OpenBlock()
{
  Scope scope = {-1, -1};
  scopeStack.push_back(scope);
  nesting++;
}

/*
 * CloseBlock():  decrement nesting level counter and remove the current
 * block from the stack.  called when exitting from a program, procedure,
 * function or a record definition. each element is checked to see if it is
 * used in the block
 */
void CloseBlock()
{
  scopeStack.pop_back();
  nesting--;
}

/*
 * IsAttr(): return the index to the searched attribute in attrarray if it is
 * in (nonzero).  otherwise, return false.
 */
int IsAttr(int st_ptr, int attr_num)
{
  if (st_ptr >= symbolTable.size())
  {
    printf("DEBUG--Symbol table entry %d does not exist.\n", st_ptr);
    assert(0);
  }
  if (symbolTable[st_ptr].attrs.find(attr_num) != symbolTable[st_ptr].attrs.end())
    return 1;
  else
    return 0;
}

/*
 * GetAttr(): return the specified attribute value for a symbol table entry
 * if found.  otherwise, report error.  Note, this error is not the fault of
 * source program but of the compiler writer.  It is printed for testing and
 * debugging.
 */
uintptr_t GetAttr(int st_ptr, int attr_num)
{
  if (st_ptr >= symbolTable.size())
  {
    printf("DEBUG--Symbol table entry %d does not exist.\n", st_ptr);
    assert(0);
  }
  if (symbolTable[st_ptr].attrs.find(attr_num) == symbolTable[st_ptr].attrs.end())
  {
    printf("DEBUG--The wanted attribute number %d does not exist\n", attr_num);
    return 0;
  }
  return symbolTable[st_ptr].attrs[attr_num];
}

/*
 * SetAttr(): set attribute.  if the attribute is already there,  print
 * debugging message. attributes for a symbol table entry are sorted by their
 * attr_num.
 */
void SetAttr(int st_ptr, int attr_num, uintptr_t attr_val)
{
  if (st_ptr >= symbolTable.size())
  {
    printf("DEBUG--Symbol table entry %d does not exist.\n", st_ptr);
    assert(0);
  }
  if (symbolTable[st_ptr].attrs.find(attr_num) != symbolTable[st_ptr].attrs.end() && attr_num != IS_USED_ATTR)
  {
    printf("DEBUG--The attribute number %d to be added already exists\n", attr_num);
    return;
  }
  symbolTable[st_ptr].attrs[attr_num] = attr_val;
}

const char *kind_name[] = {"variable", "function", "ref_arg", "val_arg", "class", "program", "array"};

std::string getNodeString(tree nd)
{
  switch (NodeKind(nd))
  {
  case EXPRNode:
    return getNodeOpString(nd);
  case NUMNode:
    return "NUMNode(" + std::to_string(IntVal(nd)) + ")";
  default:
    printf("DEBUG--Unprintable node kind %s\n", kind_name[NodeKind(nd)]);
    assert(0);
  }
}

std::string getTypeString(tree nd)
{
  switch (NodeKind(nd))
  {
  case INTEGERTNode:
    return "int";
  case EXPRNode:
  {
    assert(NodeOp(nd) == TypeIdOp);
    std::string type;
    if (NodeKind(LeftChild(nd)) == INTEGERTNode)
      type = "int";
    else
    {
      assert(NodeKind(LeftChild(nd)) == STNode);
      int idx = IntVal(LeftChild(nd));
      assert(idx > 0);
      type = getString(GetAttr(idx, NAME_ATTR));
    }
    for (tree indexOp = RightChild(nd); NodeOp(indexOp) == IndexOp; indexOp = RightChild(indexOp))
      type += "[]";
    return type;
  }
  case DUMMYNode:
    return "void";
  default:
    printf("DEBUG--Unprintable node kind %s\n", kind_name[NodeKind(nd)]);
    assert(0);
  }
}

const char *getDimensionsString(std::vector<int> *dims)
{
  static char buf[11];
  std::string str;

  assert(dims != NULL);
  for (int dim : *dims)
    str += std::to_string(dim) + " ";
  if (str.length() > 0)
    str.erase(str.length() - 1, 1);
  snprintf(buf, 11, "%s", str.c_str());
  return buf;
}

/*
 * STPrint():  print symbol table.  This routine is incomplete, please fill
 * in details.
 */
void STPrint(FILE *table, bool printLineNo)
{
  int i, attr_num, attr_ptr, type_attr;
  uintptr_t attr_val;
  int treeval = 0;
  tree ptrTree;

  table = stdout;
  fprintf(table, "**************** SYMBOL TABLE ****************\n\n");
  if (printLineNo)
    fprintf(table, "%3s%11s%11s%11s%11s%11s%14s%11s%11s%11s%11s%11s\n\n", "", "Name", "Kind", "Nest-Level", "Predefined", "Type", "Init-Node", "Offset", "Dimensions", "Arguments", "LLVMObject", "LineNo");
  else
    fprintf(table, "%3s%11s%11s%11s%11s%11s%14s%11s%11s%11s%11s\n\n", "", "Name", "Kind", "Nest-Level", "Predefined", "Type", "Init-Node", "Offset", "Dimensions", "Arguments", "LLVMObject");

  for (i = 0; i < symbolTable.size(); i++)
  {
    fprintf(table, "%3d", i);
    for (attr_num = NAME_ATTR; attr_num <= LINENO_ATTR; attr_num++)
    {
      if ((attr_ptr = IsAttr(i, attr_num)))
      {
        attr_val = GetAttr(i, attr_num);
        switch (attr_num)
        {
        case NAME_ATTR:
          fprintf(table, "%11s", getString(attr_val));
          break;
        case OFFSET_ATTR:
        case NEST_ATTR:
          fprintf(table, "%11lu", attr_val);
          break;
        case INIT_ATTR:
        {
          std::string node_str = attr_val ? getNodeString((tree)attr_val) : "";
          fprintf(table, "%14s", node_str.c_str());
        }
        break;
        case TYPE_ATTR:
        {
          std::string type_str = attr_val ? getTypeString((tree)attr_val) : "";
          fprintf(table, "%11s", type_str.c_str());
        }
        break;
        case PREDE_ATTR:
          if (attr_val == true)
            fprintf(table, "%11s", "yes");
          else
            fprintf(table, "%11s", "no");
          break;
        case KIND_ATTR:
          fprintf(table, "%11s", kind_name[attr_val - 1]);
          break;
        case DIMEN_ATTR:
          fprintf(table, "%11s", getDimensionsString((std::vector<int> *)attr_val));
          break;
        case ARGNUM_ATTR:
          fprintf(table, "%11lu", attr_val);
          break;
        case OBJECT_ATTR:
          if (attr_val)
          {
            fprintf(table, "%11s", ((llvm::Value *)attr_val)->getName().str().c_str());
          }
          else
            fprintf(table, "%11s", "null");
          break;
        case LINENO_ATTR:
          if (printLineNo)
            fprintf(table, "%11lu", attr_val);
          break;
        case IS_USED_ATTR:
          fprintf(table, "%11s", attr_val ? "Yes" : "No");
          break;
        }
      }
      else
      {
        if (attr_num == INIT_ATTR)
        {
          fprintf(table, "%14s", " ");
        }
        else
        {
          fprintf(table, "%11s", " ");
        }
      }
    }
    fprintf(table, "\n");
  }
}

void ErrorMessage(int type, int lineno, int id, int seq)
{
  if (!printSemanticErrors)
    return;

  printf("[Semantic Error] line %d: ", lineno);
  switch (type)
  {
  case REDECLARATION:
    printf("symbol %s is previously redeclared.\n", getString(id));
    break;
  case UNDECLARED:
    printf("symbol %s is undeclared.\n", getString(id));
    break;
  case NOT_USED:
    printf("variable %s is not used anywhere.\n", getString(id));
    break;
  case TYPE_MISMATCH:
    printf("symbol %s the type does not allow this operation.\n", getString(id));
    break;
  case ARGUMENTS_NUM:
    printf("function %s argument number is different from declared parameter number.\n", getString(id));
    break;
  case DIMENSIONS_NUM:
    printf("array %s accessed with an inappropriate number of dimensions.\n", getString(id));
    break;
  case BOUND_NOT_CONST:
    printf("symbol %s is not a constant so can't be used as an array bound.\n", getString(id));
    break;
  case NOT_A_FIELD:
    printf("symbol %s is not a field in the class.\n", getString(id));
    break;
  case NOT_AN_ARRAY:
    printf("symbol %s not an array so can't be indexed.\n", getString(id));
    break;
  case NOT_A_VARIABLE:
    printf("symbol %s not a variable so has no value.\n", getString(id));
    break;
  case NOT_A_FUNCTION:
    printf("symbol %s not a function so can't be called.\n", getString(id));
    break;
  case NOT_A_TYPE:
    printf("symbol %s is not a type.\n", getString(id));
    break;
  default:
    printf("error type: %d.\n", type);
  }
}
