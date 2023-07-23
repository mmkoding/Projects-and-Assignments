""" 
A very quick-n-dirty script to print nestrees in a formatted way. Not high-perf!

Author: Deniz Sayın
"""

INDENT_SPACES = 2

if __name__ == '__main__':
    line = input()
    dp = line.find('$')
    if dp == -1:
        raise ValueError("Where's the $? Please paste the input as is!")
    line = line[dp+2:]

    last_open = False
    last_comma = False
    indent = 0
    for ch in line:
        if last_comma:
            assert ch == ' '
            last_comma = False
            continue

        if last_open and ch != ']':
            indent += INDENT_SPACES
            print()
            print(indent * ' ', end='')
        
        if ch == ',':
            print(',')
            print(indent * ' ', end='')
        elif ch == ']':
            if last_open:
                print(']', end='')
            else:
                indent -= INDENT_SPACES
                print()
                print(indent * ' ', end='')
                print(']', end='')
        else:
            print(ch, end='')
        
        last_open = ch == '['
        last_comma = ch == ','
    print()

