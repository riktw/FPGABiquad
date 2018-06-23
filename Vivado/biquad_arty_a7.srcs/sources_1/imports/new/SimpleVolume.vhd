library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;   


entity SimpleVolume is
  Port 
  ( 
    MCLK                : in std_logic;
    DataLeftIn          : in std_logic_vector(23 downto 0);
    DataRightIn         : in std_logic_vector(23 downto 0);
    DataLeftInValid     : in std_logic;
    DataRightInValid    : in std_logic;
    DataLeftOut         : out std_logic_vector(23 downto 0);
    DataRightOut        : out std_logic_vector(23 downto 0);
    sel                 : in std_logic
  );
end SimpleVolume;

architecture Behavioral of SimpleVolume is
    signal DataLeftValid, DataRightValid : std_logic;
begin

process (MCLK)
    variable DataRight : signed(23 downto 0);
begin
    if rising_edge(MCLK) then
        if DataRightInValid = '1' then
            DataRight := signed(DataRightIn);
            if sel = '1' then
                DataRight := DataRight;
            else
                DataRight := shift_right(DataRight, 1);
            end if;
        end if;
        DataRightOut <= std_logic_vector(DataRight);
    end if;
end process;

process (MCLK)
    variable DataLeft : signed(23 downto 0);
begin
    if rising_edge(MCLK) then
        if DataLeftInValid = '1' then
            DataLeft := signed(DataLeftIn);
            if sel = '1' then
                DataLeft := DataLeft;
            else
                DataLeft := shift_right(DataLeft, 1);
            end if;
        end if;    
        DataLeftOut <= std_logic_vector(DataLeft);
    end if;
end process;

end Behavioral;
