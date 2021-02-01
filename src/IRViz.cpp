#include <iostream>
#include <sstream>
#include <fstream>

#include "IRViz.h"

#include "AssociativeOpsTable.h"
#include "Associativity.h"
#include "IROperator.h"
#include "Module.h"
#include "Target.h"

namespace Halide {

using std::ostream;
using std::ostringstream;
using std::string;
using std::vector;

namespace Internal {

IRViz::~IRViz() {
}

IRViz::IRViz() 
{
}

void IRViz::visualize(Expr expr)
{
    stream<<"digraph G {\nnode [shape=record];\n";
    viz(expr);
    stream<<"\n}\n";
}
void IRViz::png(Expr expr,std::string filename)
{
	visualize(expr);
    std::string m_dot="temp.dot";
    std::ofstream dot(m_dot);
    dot<<stream.str();


    auto retcode = system(("dot " + m_dot + " -Tpng -o " + filename).c_str());
    if (retcode != 0)
    {
        std::cout << "to_png failed! To png depends on dot! Please make sure graphviz have been installed.";
    }
}

void IRViz::viz(Expr ir) {
    ir.accept(this);
}

void IRViz::viz(Stmt ir) {
    ir.accept(this);
}

void IRViz::visit(const IntImm *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"IntImm|"
           << op->type << "|"
           << op->value << "\"];\n";
}

void IRViz::visit(const UIntImm *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"UIntImm|"
           << op->type << "|"
           << op->value << "\"];\n";
}

void IRViz::visit(const FloatImm *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"FloatImm|"
           << op->type << "|";
    switch (op->type.bits()) {
    case 64:
        stream << op->value;
        break;
    case 32:
        stream << op->value << 'f';
        break;
    case 16:
        stream << op->value << 'h';
        break;
    default:
        internal_error << "Bad bit-width for float: " << op->type << "\n";
    }
    stream << "\"];\n";
}

void IRViz::visit(const StringImm *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"StringImm|"
           << op->type << "|";
    stream << "\\\"";
    for (size_t i = 0; i < op->value.size(); i++) {
        unsigned char c = op->value[i];
        if (c >= ' ' && c <= '~' && c != '\\' && c != '"') {
            stream << c;
        } else {
            stream << '\\';
            switch (c) {
            case '"':
                stream << "\\\"";
                break;
            case '\\':
                stream << '\\';
                break;
            case '\t':
                stream << 't';
                break;
            case '\r':
                stream << 'r';
                break;
            case '\n':
                stream << 'n';
                break;
            default:
                string hex_digits = "0123456789ABCDEF";
                stream << 'x' << hex_digits[c >> 4] << hex_digits[c & 0xf];
            }
        }
    }
    stream << "\\\"";
    stream << "\"];\n";
}

void IRViz::visit(const Cast *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Cast|"
           << op->type;
    stream << "\"];\n";
    stream << (uint64_t) op->value.get() << "->" << (uint64_t) op << ";";
}

void IRViz::visit(const Variable *op) {
    // omit the type
    // stream << op->name << "." << op->type;
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Variable|"
           << op->type << "|";
    stream << op->name;
    stream << "\"];\n";
}

void IRViz::visit(const Add *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Add|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const Sub *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Sub|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const Mul *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Mul|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const Div *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Div|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const Mod *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Mod|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const Min *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Min|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const Max *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Max|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const EQ *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"EQ|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const NE *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"NE|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const LT *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"LT|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const LE *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"LE|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const GT *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"GT|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const GE *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"GE|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const And *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"And|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const Or *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Or|"
           << op->type << "\"];\n";
    auto a = op->a.get();
    auto b = op->b.get();
    stream << (uint64_t) a << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b << "->" << (uint64_t) op << ";";
    viz(op->a);
    viz(op->b);
}

void IRViz::visit(const Not *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Not|"
           << op->type << "\"];\n";
    stream << (uint64_t) op->a.get() << "->" << (uint64_t) op << ";";
}

void IRViz::visit(const Select *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Select|"
           << op->type << "\"];\n";
    auto &a = op->condition;
    auto &b = op->true_value;
    auto &c = op->true_value;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) c.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
    viz(c);
}

void IRViz::visit(const Load *op) {
    const bool has_pred = !is_one(op->predicate);
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Load|"
           << op->type << "\"];\n";
    if (has_pred) {
        auto &a = op->predicate;
        stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
        viz(a);
    }
}

void IRViz::visit(const Ramp *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Ramp|"
           << op->type << "\"];\n";
    auto &a = op->base;
    auto &b = op->stride;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
}

void IRViz::visit(const Broadcast *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Broadcast|"
           << op->type << "\"];\n";
    stream << (uint64_t) op->value.get() << "->" << (uint64_t) op << ";";
}

void IRViz::visit(const Call *op) {
    // TODO: Print indication of C vs C++?
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Call|"
           << op->type
           << "|" << op->name << "|"
           << "\"];\n";
    for (auto &arg : op->args) {
        stream << (uint64_t) arg.get() << "->" << (uint64_t) op << ";";
        viz(arg);
    }
}

void IRViz::visit(const Let *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Let|"
           << op->type
           << "|" << op->name << "|"
           << "\"];\n";
    auto &a = op->value;
    auto &b = op->body;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
}

void IRViz::visit(const LetStmt *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"LetStmt|"
           << "|" << op->name << "|"
           << "\"];\n";
    auto &a = op->value;
    auto &b = op->body;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
}

void IRViz::visit(const AssertStmt *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"AssertStmt|"
           << "\"];\n";
    auto &a = op->condition;
    auto &b = op->message;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
}

void IRViz::visit(const ProducerConsumer *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"ProducerConsumer|";
    if (op->is_producer) {
        stream << "produce " << op->name << "";
    } else {
        stream << "consume " << op->name << "";
    }
    stream << "\"];\n";
    auto &a = op->body;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    viz(a);
}

void IRViz::visit(const For *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"For"
           << "|" << op->name << "|"
           << "\"];\n";
    auto &a = op->min;
    auto &b = op->extent;
    auto &c = op->body;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) c.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
    viz(c);
}

void IRViz::visit(const Acquire *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Acquire"
           << "\"];\n";
    auto &a = op->semaphore;
    auto &b = op->count;
    auto &c = op->body;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) c.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
    viz(c);
}

void IRViz::visit(const Store *op) {
    const bool has_pred = !is_one(op->predicate);
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Store|"
           << "\"];\n";
    if (has_pred) {
        auto &a = op->predicate;
        stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
        viz(a);
    }
    auto &a = op->index;
    auto &b = op->value;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
}

void IRViz::visit(const Provide *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Provide|"
           << "|" << op->name << "|"
           << "\"];\n";
    for (auto &arg : op->args) {
        stream << (uint64_t) arg.get() << "->" << (uint64_t) op << ";";
        viz(arg);
    }
    for (auto &arg : op->values) {
        stream << (uint64_t) arg.get() << "->" << (uint64_t) op << ";";
        viz(arg);
    }
}

void IRViz::visit(const Allocate *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Allocate|"
           << "|" << op->name << "|"
           << "\"];\n";
    for (auto &arg : op->extents) {
        stream << (uint64_t) arg.get() << "->" << (uint64_t) op << ";";
        viz(arg);
    }
    if (!is_one(op->condition)) {
        auto &a = op->condition;
        stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
        viz(a);
    }
    auto &b = op->body;
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(b);
}

void IRViz::visit(const Free *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Free"
           << "|" << op->name << "|"
           << "\"];\n";
}

void IRViz::visit(const Realize *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Realize|"
           << "|" << op->name << "|"
           << "\"];\n";
    for (auto &arg : op->bounds) {
        auto &a = arg.min;
        auto &b = arg.extent;
        stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
        stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
        viz(a);
        viz(b);
    }
    if (!is_one(op->condition)) {
        auto &a = op->condition;
        stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
        viz(a);
    }
    auto &b = op->body;
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(b);
}

void IRViz::visit(const Prefetch *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Prefetch|"
           << "|" << op->name << "|"
           << "\"];\n";
    for (auto &arg : op->bounds) {
        auto &a = arg.min;
        auto &b = arg.extent;
        stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
        stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
        viz(a);
        viz(b);
    }
    if (!is_one(op->condition)) {
        auto &a = op->condition;
        stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
        viz(a);
    }
    auto &b = op->body;
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(b);
}

void IRViz::visit(const Block *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Block|"
           << "\"];\n";

    auto &a = op->first;
    auto &b = op->rest;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    stream << (uint64_t) b.get() << "->" << (uint64_t) op << ";";
    viz(a);
    viz(b);
}

void IRViz::visit(const Fork *op) {
    vector<Stmt> stmts;
    stmts.push_back(op->first);
    Stmt rest = op->rest;
    while (const Fork *f = rest.as<Fork>()) {
        stmts.push_back(f->first);
        rest = f->rest;
    }
    stmts.push_back(rest);
    //TODO
    // for (Stmt s : stmts) {
    //     stream << "{\n";
    //     indent += 2;
    //     print(s);
    //     indent -= 2;
    //     do_indent();
    //     stream << "} ";
    // }
    // stream << "\n";
}

void IRViz::visit(const IfThenElse *op) {
    // do_indent();
    // while (1) {
    //     stream << "if (" << op->condition << ") {\n";
    //     indent += 2;
    //     print(op->then_case);
    //     indent -= 2;

    //     if (!op->else_case.defined()) {
    //         break;
    //     }

    //     if (const IfThenElse *nested_if = op->else_case.as<IfThenElse>()) {
    //         do_indent();
    //         stream << "} else ";
    //         op = nested_if;
    //     } else {
    //         do_indent();
    //         stream << "} else {\n";
    //         indent += 2;
    //         print(op->else_case);
    //         indent -= 2;
    //         break;
    //     }
    // }

    // do_indent();
    // stream << "}\n";
}

void IRViz::visit(const Evaluate *op) {
    stream << (uint64_t)(op)
           << "[ shape=\"record\" "
           << "label=\"Evaluate|";
    stream << "\"];\n";
    auto &a = op->value;
    stream << (uint64_t) a.get() << "->" << (uint64_t) op << ";";
    viz(a);
}

void IRViz::visit(const Shuffle *op) {
    //TODO
    // if (op->is_concat()) {
    //     stream << "concat_vectors(";
    //     print_list(op->vectors);
    //     stream << ")";
    // } else if (op->is_interleave()) {
    //     stream << "interleave_vectors(";
    //     print_list(op->vectors);
    //     stream << ")";
    // } else if (op->is_extract_element()) {
    //     stream << "extract_element(";
    //     print_list(op->vectors);
    //     stream << ", " << op->indices[0];
    //     stream << ")";
    // } else if (op->is_slice()) {
    //     stream << "slice_vectors(";
    //     print_list(op->vectors);
    //     stream << ", " << op->slice_begin() << ", " << op->slice_stride() << ", " << op->indices.size();
    //     stream << ")";
    // } else {
    //     stream << "shuffle(";
    //     print_list(op->vectors);
    //     stream << ", ";
    //     for (size_t i = 0; i < op->indices.size(); i++) {
    //         print(op->indices[i]);
    //         if (i < op->indices.size() - 1) {
    //             stream << ", ";
    //         }
    //     }
    //     stream << ")";
    // }
}

}  // namespace Internal
}  // namespace Halide
