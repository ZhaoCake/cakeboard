module top (
    input clk,
    input rstn,
    input [7:0] sw,
    output reg [7:0] led
);

    // 简单的测试逻辑：LED显示开关的值
    always @(posedge clk or negedge rstn) begin
        if (!rstn)
            led <= 8'h00;
        else
            led <= sw;
    end

endmodule 