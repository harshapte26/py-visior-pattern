from visitor_pattern import main

# res = main("*3")
# print(res)

def test_addition ():
    res = main("3+4")
    assert res['result'] == 7

def test_subtraction ():
    res = main("16-8")
    assert res['result'] == 8

def test_multiplication ():
    res = main("5*3")
    assert res['result'] == 15

def test_addition ():
    res = main("9/3")
    assert res['result'] == 3

def test_expression_only_add_subtract():
    s = "3-5+6-2+9"
    res = main(s)
    assert res['result'] == 11

def test_expression_only_multiply_divide():
    s = "8*3/4"
    res = main(s)
    assert res['result'] == 6

def test_complex_expression():
    s = "11-8/4*3+2"
    res = main(s)
    assert res['result'] == 7

def test_empty_input ():
    res = main("")
    assert res == "Invalid Input"