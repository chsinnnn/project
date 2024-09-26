library verilog;
use verilog.vl_types.all;
entity FA_ALU32 is
    port(
        a               : in     vl_logic_vector(31 downto 0);
        b               : in     vl_logic_vector(31 downto 0);
        cin             : in     vl_logic;
        sum             : out    vl_logic_vector(31 downto 0);
        andNum          : out    vl_logic_vector(31 downto 0);
        orNum           : out    vl_logic_vector(31 downto 0);
        sltNum          : out    vl_logic_vector(31 downto 0)
    );
end FA_ALU32;
