#ifndef NEO_HALIDE_IR_VIZ_H
#define NEO_HALIDE_IR_VIZ_H

/** \file
 * This op would try to convert a expression to a DAG.
 */

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "IRVisitor.h"
#include "Module.h"

namespace Halide {

namespace Internal {

struct AssociativePattern;
struct AssociativeOp;

/** An IRVisitor that emits IR to the given output stream in a human
 * readable form. Can be subclassed if you want to modify the way in
 * which it prints.
 */
class IRViz : public IRVisitor {
public:
    virtual ~IRViz();

    IRViz();

    void visualize(Expr);
    void png(Expr,std::string);
protected:
    /** emit an expression on the output stream */
    void viz(Expr);

    /** emit a statement on the output stream */
    void viz(Stmt);

    /** The stream we're outputting on */
    std::ostringstream stream;
    // /** a map for pointer-> name*/
    // std::map<IRNode *, std::string> name_map;
    void visit(const IntImm *) override;
    void visit(const UIntImm *) override;
    void visit(const FloatImm *) override;
    void visit(const StringImm *) override;
    void visit(const Cast *) override;
    void visit(const Variable *) override;
    void visit(const Add *) override;
    void visit(const Sub *) override;
    void visit(const Mul *) override;
    void visit(const Div *) override;
    void visit(const Mod *) override;
    void visit(const Min *) override;
    void visit(const Max *) override;
    void visit(const EQ *) override;
    void visit(const NE *) override;
    void visit(const LT *) override;
    void visit(const LE *) override;
    void visit(const GT *) override;
    void visit(const GE *) override;
    void visit(const And *) override;
    void visit(const Or *) override;
    void visit(const Not *) override;
    void visit(const Select *) override;
    void visit(const Load *) override;
    void visit(const Ramp *) override;
    void visit(const Broadcast *) override;
    void visit(const Call *) override;
    void visit(const Let *) override;
    void visit(const LetStmt *) override;
    void visit(const AssertStmt *) override;
    void visit(const ProducerConsumer *) override;
    void visit(const For *) override;
    void visit(const Acquire *) override;
    void visit(const Store *) override;
    void visit(const Provide *) override;
    void visit(const Allocate *) override;
    void visit(const Free *) override;
    void visit(const Realize *) override;
    void visit(const Block *) override;
    void visit(const Fork *) override;
    void visit(const IfThenElse *) override;
    void visit(const Evaluate *) override;
    void visit(const Shuffle *) override;
    void visit(const Prefetch *) override;
};
}  // namespace Internal
}  // namespace Halide

#endif  //NEO_HALIDE_IR_VIZ_H
