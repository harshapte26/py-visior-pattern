package com.nyu.edu;

public class Constant implements Expression {

	double constant;

	public Constant(double constant) {
		this.constant = constant;
	}

	@Override
	public void accept(Visitor visitor) {
		visitor.visit(this);
	}

}
