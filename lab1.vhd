library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity lab1 is
	port(
		clk : in std_logic := '0';
		reset_n : in std_logic :='0';
		leds : out std_logic_vector(7 downto 0);
		button : in std_logic_vector(1 downto 0); 
		switch : in std_logic_vector(9 downto 0));
end entity lab1;

architecture rit of lab1 is

    component lab1_sys is
        port (
				clk_clk                            : in  std_logic                    := 'X';             -- clk
				reset_reset_n                      : in  std_logic                    := 'X';             -- reset_n
            leds_external_connection_export    : out std_logic_vector(7 downto 0);                    -- export
            button_external_connection_export  : in  std_logic_vector(1 downto 0) := (others => 'X'); -- export
            switchs_external_connection_export : in  std_logic_vector(9 downto 0) := (others => 'X')  -- export
        );
    end component lab1_sys;
begin
    u0 : component lab1_sys
        port map (
				clk_clk                            => clk,     --  clk.clk
            reset_reset_n                      => reset_n, --  reset.reset_n
            leds_external_connection_export    => leds,    --  leds_external_connection.export
            button_external_connection_export  => button,  --  button_external_connection.export
            switchs_external_connection_export => switch   --  switchs_external_connection.export
        );
end architecture;


    

    
