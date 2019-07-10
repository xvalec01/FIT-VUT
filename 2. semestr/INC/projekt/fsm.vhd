-- fsm.vhd: Finite State Machine
-- Author(s): David Valecký (xvalec01@stud.fit.vutbr.cz)
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (NUMBER_1, NUMBER_2, NUMBER_3, NUMBER_4A, NUMBER_4B, NUMBER_5A,
   NUMBER_5B, NUMBER_6A, NUMBER_6B, NUMBER_7A, NUMBER_7B, NUMBER_8A, NUMBER_8B, NUMBER_9A, 
   NUMBER_9B, NUMBER_10B, TEST, ERROR_MSG, SUCCES_MSG, FAIL, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
  if (RESET = '1') then
    present_state <= NUMBER_1;
  elsif (CLK'event AND CLK = '1') then
    present_state <= next_state;
  end if;
end process sync_logic;
-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
  case (present_state) is
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_1 =>
    next_state <= NUMBER_1;
    if (KEY(9) = '1') then
      next_state <= NUMBER_2;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;	
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_2 =>
    next_state <= NUMBER_2;
    if (KEY(3) = '1') then
      next_state <= NUMBER_3;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_3 =>
    next_state <= NUMBER_3;
    if (KEY(7) = '1') then
      next_state <= NUMBER_4A;
    elsif (KEY(3) = '1') then
      next_state <= NUMBER_4B;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_4A =>
    next_state <= NUMBER_4A;
    if (KEY(9) = '1') then
      next_state <= NUMBER_5A;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_5A =>
    next_state <= NUMBER_5A;
    if (KEY(6) = '1') then
      next_state <= NUMBER_6A;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_6A =>
    next_state <= NUMBER_6A;
    if (KEY(9) = '1') then
      next_state <= NUMBER_7A;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_7A =>
    next_state <= NUMBER_7A;
    if (KEY(3) = '1') then
      next_state <= NUMBER_8A;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_8A =>
    next_state <= NUMBER_8A;
    if (KEY(9) = '1') then
      next_state <= NUMBER_9A;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_9A =>
    next_state <= NUMBER_9A;
    if (KEY(7) = '1') then
      next_state <= TEST;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when TEST =>
    next_state <= TEST;
    if (KEY(15) = '1') then
      next_state <= SUCCES_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_4B =>
    next_state <= NUMBER_4B;
    if (KEY(4) = '1') then
      next_state <= NUMBER_5B;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_5B =>
    next_state <= NUMBER_5B;
    if (KEY(7) = '1') then
      next_state <= NUMBER_6B;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_6B =>
    next_state <= NUMBER_6B;
    if (KEY(7) = '1') then
      next_state <= NUMBER_7B;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_7B =>
    next_state <= NUMBER_7B;
    if (KEY(0) = '1') then
      next_state <= NUMBER_8B;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_8B =>
    next_state <= NUMBER_8B;
    if (KEY(4) = '1') then
      next_state <= NUMBER_9B;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_9B =>
    next_state <= NUMBER_9B;
    if (KEY(8) = '1') then
      next_state <= NUMBER_10B;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when NUMBER_10B =>
    next_state <= NUMBER_10B;
    if (KEY(0) = '1') then
      next_state <= TEST;
    elsif (KEY(15) = '1') then
      next_state <= ERROR_MSG;
    elsif (KEY(14 downto 0) /= "000000000000000") then 
      next_state <= FAIL;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when FAIL =>
    next_state <= FAIL;
    if (KEY(15) = '1') then
      next_state <= ERROR_MSG; 
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when SUCCES_MSG =>
    next_state <= SUCCES_MSG;
    if (CNT_OF = '1') then
      next_state <= FINISH;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when ERROR_MSG =>
    next_state <= ERROR_MSG;
    if (CNT_OF = '1') then
      next_state <= FINISH;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when FINISH =>
    next_state <= FINISH;
    if (KEY(15) = '1' ) then
      next_state <= NUMBER_1;
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when others =>
    next_state <= NUMBER_1;

  end case;

end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin

  FSM_CNT_CE     <= '0';
  FSM_MX_MEM     <= '0';
  FSM_MX_LCD     <= '0';
  FSM_LCD_WR     <= '0';
  FSM_LCD_CLR    <= '0';

  case (present_state) is
  -- - - - - - - - - - - - - - - - - - - - - - -
  when SUCCES_MSG =>
    FSM_CNT_CE     <= '1';
    FSM_MX_MEM     <= '1';
    FSM_MX_LCD     <= '1';
    FSM_LCD_WR     <= '1';
  -- - - - - - - - - - - - - - - - - - - - - - -
  when ERROR_MSG =>
    FSM_CNT_CE     <= '1';
    FSM_MX_MEM     <= '0';
    FSM_MX_LCD     <= '1';
    FSM_LCD_WR     <= '1';
  -- - - - - - - - - - - - - - - - - - - - - - -
  when FINISH =>
    if (KEY(15) = '1') then
      FSM_LCD_CLR    <= '1';
    end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
  when others =>
  if (KEY(14 downto 0) /= "000000000000000") then
    FSM_LCD_WR     <= '1';
  end if;
  if (KEY(15) = '1') then
    FSM_LCD_CLR    <= '1';
  end if;  
  end case;
end process output_logic;

end architecture behavioral;