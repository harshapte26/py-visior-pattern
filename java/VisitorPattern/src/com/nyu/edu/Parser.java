package com.nyu.edu;

public class Parser {

	public static Expression Parse(String t) {

		for (int i = t.length() - 1; i >= 0; i--) {
			if (t.charAt(i) == '+' || t.charAt(i) == '-') {
				return new Operator(t.charAt(i), Parse(t.substring(0, i)), Parse(t.substring(i + 1)));
			}
		}

		for (int i = t.length() - 1; i >= 0; i--) {
			if (t.charAt(i) == '*' || t.charAt(i) == '/') {
				return new Operator(t.charAt(i), Parse(t.substring(0, i)), Parse(t.substring(i + 1)));
			}
		}

		for (int i = t.length() - 1; i >= 0; i--) {
			if (t.charAt(i) == '^') {
				return new Operator(t.charAt(i), Parse(t.substring(0, i)), Parse(t.substring(i + 1)));
			}
		}

		return new Constant(Integer.parseInt(t));
	}
}
