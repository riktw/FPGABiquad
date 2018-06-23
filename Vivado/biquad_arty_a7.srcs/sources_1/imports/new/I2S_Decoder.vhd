library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;   

entity I2S_Decoder is
    port
    (
        SCLK            : in std_logic;
        SDIN            : in std_logic;
        LRCLK           : in std_logic;
        DataLeft        : out std_logic_vector(23 downto 0);
        DataRight       : out std_logic_vector(23 downto 0);
        DataLeftReady   : out std_logic;
        DataRightReady  : out std_logic
    );
end I2S_Decoder;

architecture rtl of I2S_Decoder is

    signal lrdetect1, lrdetect2, lrdetect : std_logic;
    signal counter : integer range 0 to 64;
    signal shifter : std_logic_vector(31 downto 0);
    
    function reverse_any_vector (a: in std_logic_vector)
    return std_logic_vector is
      variable result: std_logic_vector(a'RANGE);
      alias aa: std_logic_vector(a'REVERSE_RANGE) is a;
    begin
      for i in aa'RANGE loop
        result(i) := aa(i);
      end loop;
      return result;
    end; -- function reverse_any_vector

begin

    edge_detect1: process (SCLK)
    begin
        if rising_edge(SCLK) then
            lrdetect1 <= LRCLK;
        end if;
    end process;

    edge_detect2: process (SCLK)
    begin
        if rising_edge(SCLK) then
            lrdetect2 <= lrdetect1;
        end if;
    end process;
    
    lrdetect <= lrdetect1 xor lrdetect2;
    
    counting: process (SCLK)
    begin
        if falling_edge(SCLK) then
            if lrdetect = '1' then
                counter <= 0;
            else
                counter <= counter + 1;
            end if;
        end if;
    end process;

    shifting: process (SCLK)
    begin
        if rising_edge(SCLK) then
            if lrdetect = '1' then
                shifter <= (others=>'0');
            end if;
            if(counter < 32) then
                shifter(31 - counter) <= SDIN;
            end if;
        end if;
    end process;


    leftdata: process (SCLK)
    begin
        if rising_edge(SCLK) then
            if (lrdetect1 = '1') and (lrdetect = '1') then
                DataLeft(23 downto 0) <= shifter(31 downto 8);
                DataLeftReady <= '1';
            else
                DataLeftReady <= '0';
            end if;
        end if;
    end process;
    
    rightdata: process (SCLK)
    begin
        if rising_edge(SCLK) then
            if (lrdetect1 = '0') and (lrdetect = '1') then
                dataright(23 downto 0) <= shifter(31 downto 8);
                DataRightReady <= '1';
            else
                DataRightReady <= '0';
            end if;
        end if;
    end process;

end rtl;

