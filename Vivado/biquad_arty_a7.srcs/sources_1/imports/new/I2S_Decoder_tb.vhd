----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 06/03/2018 09:50:37 PM
-- Design Name: 
-- Module Name: I2Stest_tb - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;


entity I2S_Decoder_tb is
end I2S_Decoder_tb;

architecture Behavioral of I2S_Decoder_tb is

component I2Stestrik is
    generic (
    DATA_WIDTH : integer := 24;
    BITPERFRAME : integer := 64;
    DELAY_RST           : unsigned(31 downto 0) := x"00001000"
    );
    port
    (
        sw              : in std_logic_vector(3 downto 0);
        CODEC_RST       : out std_logic;
        LRCLK           : in std_logic;
        MCLK            : in std_logic;
        SCLK            : in std_logic;
        CLK100MHZ       : in std_logic;
        DAC_D0          : in std_logic;
        DAC_D1          : in std_logic;
        ADC_D0          : out std_logic;
        ADC_D1          : out std_logic
    );
end component;

constant CLK_PERIOD : time := 10 ns;
constant CLK_PERIOD_I2S : time := 42 ns;
constant CDATALEFT  : unsigned(47 downto 0) := x"0123456789AB";--:= X"123456789ABC";
constant CDATARIGHT : unsigned(47 downto 0) := X"789ABC123456";

signal        CODEC_RST       : std_logic;
signal        LRCLK           : std_logic;
signal        MCLK            : std_logic;
signal        SCLK            : std_logic;
signal        CLK100MHZ       : std_logic;
signal        SDIN            : std_logic;
signal        SDOUT           : std_logic;
signal        SDOUT2          : std_logic;
signal        DataLeft        : std_logic_vector(23 downto 0);
signal        DataRight       : std_logic_vector(23 downto 0);
signal        DataRightReady  : std_logic;
signal        DataLeftReady   : std_logic;
signal        sw              : std_logic_vector(3 downto 0);

for I2Stest_0 : I2Stestrik use entity work.I2Stestrik;

signal datacnt : unsigned(7 downto 0) := (others => '0');

begin

   I2Stest_0 : I2Stestrik port map
    (
        sw => sw,
        CODEC_RST => CODEC_RST,
        LRCLK => LRCLK,
        MCLK => MCLK,
        SCLK => MCLK,
        CLK100MHZ => CLK100MHZ,
        DAC_D0 => SDIN,
        DAC_D1 => '0',
        ADC_D0 => SDOUT,
        ADC_D1 => SDOUT2
    );

    clk_process : process
    begin
        CLK100MHZ <= '0';
        wait for CLK_PERIOD / 2;
        CLK100MHZ <= '1';
        wait for CLK_PERIOD / 2;
    end process;
    
    clk_I2S_process : process
    begin
        MCLK <= '0';
        wait for CLK_PERIOD_I2S / 2;
        MCLK <= '1';
        wait for CLK_PERIOD_I2S / 2;
    end process;
    
    lrclk_process : process
    begin
        LRCLK <= '1';
        wait for CLK_PERIOD_I2S * 64 / 2;
        LRCLK <= '0';
        wait for CLK_PERIOD_I2S * 64 / 2;
    end process;
    
    data_process : process(MCLK)
    begin
        if(falling_edge(MCLK)) then
            datacnt <= datacnt + 1;
            if datacnt = x"29" then
                datacnt <= X"00";
            end if;
            SDIN <= CDATALEFT(to_integer(datacnt));
        end if;
    end process;
    
    sim_process : process
    begin
    sw <= x"3";
    wait for 50 us;
    sw <= x"3";
    wait for 50 us;
    wait;
    end process;

end Behavioral;
