package com.nyu.edu;

public class PrettyPrintVisitor implements Visitor {

	String output;

	@Override
	public void visit(Constant c) {
		output = "(" + c.constant + ")";
	}

	@Override
	public void visit(Operator o) {

		StringBuilder result = new StringBuilder("(");

		o.left.accept(this);
		result.append(output).append(o.op);

		o.right.accept(this);
		result.append(output).append(")");

		output = result.toString();
	}
}
