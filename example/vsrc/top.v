module top (
    input clk,
    input rstn,
    // LED输出
    output reg [7:0] led_display,   // 显示用LED
    output reg [7:0] debug_led,     // 调试用LED
    output reg [7:0] status_led,    // 状态LED
    // 开关输入
    input [7:0] input_switch,       // 输入开关
    input [3:0] mode_select,        // 模式选择
    input [3:0] config_sw           // 配置开关
);

    // 计数器用于流水灯
    reg [16:0] counter;  // 适应100KHz的时钟频率
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
            
            // 每秒更新一次（时钟频率为100KHz）
            if (counter >= 100_000) begin  // 100K个周期
                counter <= 0;
                pattern <= {pattern[6:0], pattern[7]};  // 循环左移
                
                // 根据开关状态更新LED
                led_display <= pattern & input_switch;  // 与输入开关相与
                debug_led <= ~pattern;  // 反相显示
                
                // 根据模式选择更新状态LED
                case (mode_select)
                    4'b0001: status_led <= pattern << 1;  // 左移
                    4'b0010: status_led <= pattern >> 1;  // 右移
                    4'b0100: status_led <= ~pattern;      // 反相
                    4'b1000: status_led <= pattern;       // 原样
                    default: status_led <= 8'h00;         // 全灭
                endcase
            end
        end
    end

endmodule 