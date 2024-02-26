package com.nyu.edu;

public class Client {
	public static void main(String[] args) {

		String s = "3+4*2";
		Expression e = Parser.Parse(s);

		Visitor v = new ComputeVisitor();
		e.accept(v);
		System.out.println(((ComputeVisitor) v).result);

		System.out.println();

		Visitor v2 = new PrettyPrintVisitor();
		e.accept(v2);
		System.out.println(((PrettyPrintVisitor) v2).output);
	}
}
