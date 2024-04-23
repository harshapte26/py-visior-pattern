package com.nyu.edu;

public class ComputeVisitor implements Visitor {

	double result;

	@Override
	public void visit(Constant c) {
		result = c.constant;
	}

	@Override
	public void visit(Operator o) {

		o.left.accept(this);
		double l = result;

		o.right.accept(this);
		double r = result;

		switch (o.op) {
		case '^':
			result = Math.pow(l, r);
			break;
		case '+':
			result = l + r;
			break;
		case '-':
			result = l - r;
			break;
		case '*':
			result = l * r;
			break;
		case '/':
			result = l / r;
			break;
		default:
			System.out.println("invalid op " + o.op);
			System.exit(1);
		}
	}
}
