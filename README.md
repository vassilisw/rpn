# Reverse Polish Notation Calculator 


    Arithmetic Operators:
      +          Add
      -          Subtract
      *          Multiply
      /          Divide
      cla        Clear the stack and variables
      clr        Clear the stack
      clv        Clear the variables
      !          Boolean NOT
      !=         Not equal to
      %          Modulus
      ++         Increment
      --         Decrement

    Bitwise Operators:
      &          Bitwise AND
      |          Bitwise OR
      ^          Bitwise XOR
      ~          Bitwise NOT
      <<         Bitwise shift left
      >>         Bitwise shift right

    Boolean Operators:
      &&         Boolean AND
      ||         Boolean OR
      ^^         Boolean XOR

    Comparison Operators:
      <          Less than
      <=         Less than or equal to
      ==         Equal to
      >          Greater than
      >=         Greater than or equal to

    Trigonometric Functions:
      acos       Arc Cosine
      asin       Arc Sine
      atan       Arc Tangent
      cos        Cosine
      cosh       Hyperbolic Cosine
      sin        Sine
      sinh       Hyperbolic Sine
      tanh       Hyperbolic tangent

    Numeric Utilities:
      ceil       Ceiling
      floor      Floor
      round      Round
      ip         Integer part
      fp         Floating part
      abs        Absolute value
      max        Max
      min        Min

    Display Modes:
      hex        Switch display mode to hexadecimal
      dec        Switch display mode to decimal (default)
      bin        Switch display mode to binary
      oct        Switch display mode to octal

    Constants:
      e          Push e
      pi         Push Pi
      rand       Generate a random number

    Mathematic Functions:
      exp        Exponentiation
      fact       Factorial
      sqrt       Square Root
      ln         Natural Logarithm
      log        Logarithm
      pow        Raise a number to a power

    Stack Manipulation:
      pick       Pick the -n'th item from the stack
      repeat     Repeat an operation n times, e.g. '3 repeat +'
      drop       Drops the top item from the stack
      dropn      Drops n items from the stack
      dup        Duplicates the top stack item
      dupn       Duplicates the top n stack items in order
      roll       Roll the stack upwards by n
      rolld      Roll the stack downwards by n
      stack      Toggles stack display from horizontal to vertical
      swap       Swap the top 2 stack items

    Macros and Variables:
      macro      Defines a macro, e.g. 'macro kib 1024 *'
      macro      show macros (with no args)
      x=         Assigns a variable, e.g. '1024 x='

    Other:
      help       Print this message
      prec       Set precision, e.g. '4 prec'
      exit       Exit the calculator

