/**
2-1 multiplexer

input:
    sel : select signal
    a : first input signal
    b : second input signal
output :
    y : output signal
*/

module mux2( sel, a, b, y );
    parameter bitwidth = 32;
    input sel;
    input [bitwidth-1:0] a, b;
    output [bitwidth-1:0] y;

    assign y = (sel === 1'bx) ? a : (sel ? b : a);
endmodule
