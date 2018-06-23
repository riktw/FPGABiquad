library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use std.textio.all;
use ieee.std_logic_textio.all;

entity biquad is
  generic (
  	a0 : signed(31 downto 0) := x"00000111";
  	a1 : signed(31 downto 0) := x"00000223";
  	a2 : signed(31 downto 0) := x"00000111";
  	a3 : signed(31 downto 0) := x"FFE0C889";
  	a4 : signed(31 downto 0) := x"000F3BBF"
  );
  port 
  (
	  clk : in std_logic;
	  reset : in std_logic;
	  enable : in std_logic;
	  new_data : in std_logic;
	  d : in std_logic_vector(23 downto 0);
	  q : out std_logic_vector(23 downto 0)
  );
end entity;

architecture rtl of biquad is

	signal x1, x2, y1, y2 : signed(31 downto 0) := x"00000000";
	attribute mark_debug : string;
    attribute keep : string;
--    attribute mark_debug of x1     : signal is "true";
--    attribute mark_debug of x2     : signal is "true";
--    attribute mark_debug of y1     : signal is "true";
--    attribute mark_debug of y2     : signal is "true";
--	signal result : signed(63 downto 0) := x"0000000000000000"; 	

begin

	process(clk)
		variable tempinput, tempoutput : signed(31 downto 0);
		variable temp1, temp2, temp3, temp4, temp5, result : signed(63 downto 0) := x"0000000000000000"; 
	begin
		if rising_edge(clk) then
			if reset = '1' then
                if enable = '1' then
                    if new_data = '1' then
                        tempinput := resize(signed(d), 32);-- - x"00800000";
                    end if;
    
                    temp1 := shift_right((a0 * tempinput), 20);
                    temp2 := shift_right((a1 * x1), 20);
                    temp3 := shift_right((a2 * x2), 20);
                    temp4 := shift_right((a3 * y1), 20);
                    temp5 := shift_right((a4 * y2), 20);
    
                    result := temp1 + temp2 + temp3 - temp4 - temp5;
    
                    if new_data = '1' then
                        x2 <= x1;
                        x1 <= tempinput;
                        y2 <= y1;
                        y1 <= resize(result, 32);
                        q <= std_logic_vector(result(23 downto 0));
                    end if;
				else
				    q <= d;
				end if;
	
			else
				x1 <= x"00000000";
				x2 <= x"00000000";
				y1 <= x"00000000";
				y2 <= x"00000000";
			end if;
		end if;
	end process;	

end architecture;
