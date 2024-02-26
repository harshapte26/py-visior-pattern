package com.nyu.edu;

public class Operator implements Expression {

	char op;
	Expression left;
	Expression right;

	public Operator(char op, Expression left, Expression right) {
		this.op = op;
		this.left = left;
		this.right = right;
	}

	@Override
	public void accept(Visitor visitor) {
		visitor.visit(this);
	}

}
