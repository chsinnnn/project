library verilog;
use verilog.vl_types.all;
entity FA_ALU is
    port(
        a               : in     vl_logic;
        b               : in     vl_logic;
        cin             : in     vl_logic;
        control         : in     vl_logic;
        cout            : out    vl_logic;
        sum             : out    vl_logic;
        andNum          : out    vl_logic;
        orNum           : out    vl_logic;
        less            : in     vl_logic;
        sltNum          : out    vl_logic
    );
end FA_ALU;
