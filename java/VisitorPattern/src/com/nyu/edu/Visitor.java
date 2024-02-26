package com.nyu.edu;

public interface Visitor {

	public void visit(Constant constant);

	public void visit(Operator operator);

}
