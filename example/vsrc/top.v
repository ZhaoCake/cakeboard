module top (
    input clk,
    input rstn,
    output reg [7:0] led_display,   // 显示用LED
    output reg [7:0] debug_led,     // 调试用LED
    output reg [7:0] status_led     // 状态LED
);

    // 计数器用于流水灯
    reg [31:0] counter;
    reg [7:0] pattern;

    always @(posedge clk or negedge rstn) begin
        if (!rstn) begin
            counter <= 0;
            pattern <= 8'h01;
            led_display <= 0;
            debug_led <= 0;
            status_led <= 0;
        end else begin
            counter <= counter + 1;
            
            // 每秒更新一次（假设时钟频率为50MHz）
            if (counter >= 50_000_000) begin
                counter <= 0;
                pattern <= {pattern[6:0], pattern[7]};  // 循环左移
                
                // 更新不同的LED组
                led_display <= pattern;
                debug_led <= ~pattern;  // 反相显示
                status_led <= pattern << 1;  // 错位显示
            end
        end
    end

endmodule 