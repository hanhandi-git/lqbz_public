package main

import (
    "fmt"
)

func main() {
    // 数组
    var arr [5]int = [5]int{1, 2, 3, 4, 5}
    fmt.Println("Array:", arr)

    // 切片
    slice := []int{1, 2, 3}
    slice = append(slice, 4, 5) // 动态增加元素
    fmt.Println("Slice:", slice)

    // 映射
    m := make(map[string]int)
    m["Alice"] = 25
    m["Bob"] = 30
    fmt.Println("Map:", m)

    // 通道
    ch := make(chan int)
    go func() {
        ch <- 42 // 向通道发送数据
    }()
    value := <-ch // 从通道接收数据
    fmt.Println("Channel value:", value)

    // 结构体
    type Person struct {
        Name string
        Age  int
    }
    p := Person{Name: "Alice", Age: 25}
    fmt.Println("Struct:", p)

    // 接口
    var w Writer = p // 假设Person实现了Writer接口
    fmt.Println("Interface:", w)
}

// 假设的Writer接口实现
type Writer interface {
    Write(data string) error
}

func (p Person) Write(data string) error {
    fmt.Printf("%s writes: %s\n", p.Name, data)
    return nil
}