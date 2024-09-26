library verilog;
use verilog.vl_types.all;
entity Mux2to1 is
    port(
        dataA           : in     vl_logic;
        shift           : in     vl_logic;
        \signal\        : in     vl_logic;
        dataOut         : out    vl_logic
    );
end Mux2to1;
