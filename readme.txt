Cách sử dụng thư viện xử lý data truyền nhận:
- Trong file frame_porting.c
  + Implement hàm SendData_LL. Đây là hàm low-level để truyền data đi, ví dụ truyền UART, BLE...
  + Khi nhận được data đến, gọi hàm FDRV_PullData trong file frame_driver.c

- Trong file frame_receive.c
  + Modify bảng cmd_table theo ý muốn.
  + Implement các hàm callback CallbackFcn1, CallbackFcn2.
  
- Gọi hàm Frame_RecvData() liên tục, nó sẽ tự xử lý và đối chiếu cmd, gọi các hàm callback tương ứng
đã khai báo ở trên.

