library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;   

entity I2S_Encoder is
    port
    (
        SCLK            : in std_logic;
        SDOUT           : out std_logic;
        LRCLK           : in std_logic;
        DataLeft        : in std_logic_vector(23 downto 0);
        DataRight       : in std_logic_vector(23 downto 0);
        DataLeftReady   : in std_logic;
        DataRightReady  : in std_logic
    );
end I2S_Encoder;

architecture rtl of I2S_Encoder is

    signal lrdetect1, lrdetect2, lrdetect : std_logic;
    signal DataRightclocked, DataLeftclocked : std_logic_vector(23 downto 0) := (others=>'0');
    signal DataRightdelayed, DataLeftdelayed : std_logic_vector(23 downto 0) := (others=>'0');
    
    
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
    
    clockindata : process(SCLK)
    begin
        if rising_edge(SCLK) then
            if DataLeftReady = '1' then
                DataLeftclocked <= DataLeft;
            elsif DataRightReady = '1' then
                DataRightclocked <= DataRight;
            end if;
        end if;
    end process;
    
    counting: process (SCLK)
        variable counter : integer range 0 to 32;
    begin
        if falling_edge(SCLK) then
            if lrdetect = '1' then
                counter := 0;
                if lrdetect1 = '1' then
                    DataRightdelayed <= DataRightclocked;
                    SDOUT <= DataRightclocked(23-counter);
                else
                    DataLeftdelayed <= DataLeftclocked;
                    SDOUT <= DataLeftclocked(23-counter);
                end if;
            else
                counter := counter + 1;
                if counter < 24 then
                    if  lrdetect1 = '1' then
                        SDOUT <= DataRightdelayed(23-counter);
                    else
                        SDOUT <= DataLeftdelayed(23-counter);
                    end if;
                end if;
            end if;
        end if;
    end process;


end rtl;