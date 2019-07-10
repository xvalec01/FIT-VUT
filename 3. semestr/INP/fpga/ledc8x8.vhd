library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
    port(
        RESET, SMCLK	: IN    std_logic;
        LED          : OUT   std_logic_vector (0 to 7);
        ROW     		: OUT   std_logic_vector (0 to 7)
        );
end ledc8x8;

architecture main of ledc8x8 is

signal lights      : std_logic_vector(7 downto 0);
signal rows        : std_logic_vector(7 downto 0);
signal c_enable    : std_logic;
signal state       : std_logic_vector(1 downto 0)  := (others=>'0');
signal cnt         : std_logic_vector(20 downto 0) := (others=>'0');
signal clk_cnt     : std_logic_vector(14 downto 0) := (others=>'0');

begin

    state_generator: process(SMCLK, RESET)
    begin
        if RESET = '1' then
            cnt <= (others=>'0');
        elsif  rising_edge (SMCLK) then
            if cnt = "111000010000000000000" then -- 1843200
                state <= state + 1;
                cnt <= (others=>'0');
            end if;
            cnt <= cnt + 1;
        end if;
    end process state_generator;

    c_enable_generator: process(SMCLK, RESET)
    begin
  		if RESET = '1' then
  			clk_cnt <= (others=>'0');
        elsif  rising_edge (SMCLK) then
  			if clk_cnt = "111000010000000" then
  				c_enable <= '1' ;
           else
              c_enable <= '0';
           end if;

           clk_cnt <= clk_cnt + 1;
  		end if;
    end process c_enable_generator;

    row_rotation: process(RESET, c_enable, SMCLK)
    begin
        if RESET = '1' then 
    		rows <= "10000000";
    	elsif rising_edge(SMCLK) and c_enable = '1' then
    		rows <= rows(0) & rows(7 downto 1);
    	end if;
    end process row_rotation;

    decoder: process(rows,state)
    begin
      if state = "00" then
        case rows is
          when "10000000" => lights <= "10000111";
          when "01000000" => lights <= "10110011";
          when "00100000" => lights <= "10111001";
          when "00010000" => lights <= "10111101";
          when "00001000" => lights <= "10111001";
          when "00000100" => lights <= "10110011";
          when "00000010" => lights <= "10000111";
          when "00000001" => lights <= "11111111";
			 when others => lights <= "11111111";
        end case; -- row case

      elsif state = "10" then
        case rows is
          when "10000000" => lights <= "10111101";
          when "01000000" => lights <= "10111101";
          when "00100000" => lights <= "10111101";
          when "00010000" => lights <= "10111101";
          when "00001000" => lights <= "10011001";
          when "00000100" => lights <= "11000011";
          when "00000010" => lights <= "11100111";
          when "00000001" => lights <= "11111111";
			 when others => lights <= "11111111";
        end case; -- row case
        
      else
        case rows is
          when others => lights <= "11111111";
        end case;
		 end if;
    end process decoder;


    ROW <= rows;
    LED <= lights;
end main;
