library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;   


entity leftrightChange is
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
end leftrightChange;

architecture Behavioral of leftrightChange is
    signal DataLeftValid, DataRightValid : std_logic;
begin

process (MCLK)
    variable DataRight : std_logic_vector(23 downto 0);
begin
    if rising_edge(MCLK) then
        if DataRightInValid = '1' then
            if sel = '1' then
                DataRight := DataRightIn;
            else
                DataRight := DataLeftIn;
            end if;
        end if;
        DataRightOut <= DataRight;
    end if;
end process;

process (MCLK)
    variable DataLeft : std_logic_vector(23 downto 0);
begin
    if rising_edge(MCLK) then
        if DataLeftInValid = '1' then
            if sel = '1' then
                DataLeft := DataLeftIn;
            else
                DataLeft := DataRightIn;
            end if;
        end if;    
        DataLeftOut <= DataLeft;
    end if;
end process;

end Behavioral;
