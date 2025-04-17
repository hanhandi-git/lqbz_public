package main

import (
    "fmt"
    "time"
)

// 使用const定义常量
const (
    maxNumber = 10
    greeting  = "Hello, Go!"
)

// 使用type定义新类型和接口
type MyInt int

// 这个 Writer 接口声明了一个名为 Write 的方法，它:
// 接收一个 string 类型的参数 data
// 返回一个 error 类型的值
type Writer interface {
    Write(data string) error
}

// 使用struct定义结构体
type Person struct {
    Name string
    Age  int
}

// 实现Writer接口
// 方法接收者
// 方法接收者是方法的第一个参数，通常是结构体类型
// 方法接收者可以是指针类型或值类型
// 方法接收者可以有多个，用逗号分隔
// 方法接收者可以有名称，也可以没有名称
// 方法接收者可以有多个，用逗号分隔
// 方法接收者可以有名称，也可以没有名称

func (p Person) Write(data string) error {
    fmt.Printf("%s writes: %s\n", p.Name, data)
    return nil
}

// 使用map存储数据
var cache = make(map[string]int)

// 使用defer示例函数
func deferExample() {
    fmt.Println("Starting defer example")
    defer fmt.Println("This will be printed last")
    fmt.Println("This will be printed first")
}

// 使用goto的函数
func gotoExample(n int) {
    if n <= 0 {
        goto end
    }
    fmt.Println("Processing", n)
end:
    fmt.Println("End of goto example")
}

// 使用select的函数处理通道
// 通道操作通常是阻塞的，但在select语句中：
// 如果某个通道可操作，会立即执行对应的case
// 如果没有通道可操作且有default分支，则执行default
// 如果没有通道可操作且没有default分支，select会阻塞直到某个通道可操作
func selectExample(ch1, ch2, quit chan int) {
// (ch1, ch2, quit chan int) - 函数接受三个参数：ch1, ch2 和 quit，它们都是 chan int 类型
// 在Go语言中，当连续多个参数类型相同时，可以只在最后一个参数后面写出类型，前面的参数省略类型声明。这种写法等同于：
// func selectExample(ch1 chan int, ch2 chan int, quit chan int) {
    for {
        select {
		// := 是Go中的短变量声明操作符，用来声明并初始化一个新变量
		// <-ch1 是从通道ch1中接收(读取)一个值的操作
		// 如果通道ch1有值可读，那么就从ch1读取一个值，将其赋给新变量val，然后执行这个case分支中的代码。
        case val := <-ch1:
            fmt.Println("Received from channel 1:", val)
        case val := <-ch2:
            fmt.Println("Received from channel 2:", val)
        case <-quit:
            fmt.Println("Quitting select example")
            return
        default:
            fmt.Println("No channel ready")
            time.Sleep(100 * time.Millisecond)
        }
    }
}

func main() {
    // 使用var声明变量
    var number int = 5
    
    // 使用短声明
    name := "Gopher"
    
    // 使用if-else条件语句
    if number > 0 {
        fmt.Println("Number is positive")
    } else {
        fmt.Println("Number is non-positive")
    }
    
    // 使用if的简写形式
    if x := number * 2; x > 8 {
        fmt.Println("x is greater than 8")
    }
    
    // 使用for循环
    for i := 0; i < 5; i++ {
        if i == 2 {
            continue // 跳过当前循环
        }
        if i == 4 {
            break // 跳出循环
        }
        fmt.Println("Loop iteration:", i)
    }
    
    // 使用for-range
    numbers := []int{1, 2, 3, 4, 5}
    for i, v := range numbers {
        fmt.Printf("Index: %d, Value: %d\n", i, v)
    }
    
    // 使用switch
    switch number {
    case 1:
        fmt.Println("Number is 1")
    case 5:
        fmt.Println("Number is 5")
        fallthrough // 继续执行下一个case
    case 6:
        fmt.Println("This will also be printed when number is 5")
    default:
        fmt.Println("Default case")
    }
    
    // 使用结构体
    p := Person{Name: name, Age: 25}
    
    // 使用接口
	// var w Writer = p将Person实例p赋值给Writer接口变量w
	// 为什么要将Person对象赋值给Writer接口
	// 多态性：通过将Person对象赋值给Writer接口，可以利用Go的多态性。可以在不关心具体类型的情况下，使用接口来调用方法。这使得代码更加灵活和可扩展。
	// 2. 解耦合：使用接口可以降低代码之间的耦合度。调用者只需要知道Writer接口，而不需要知道具体的实现类型（在这里是Person）。这使得代码更易于维护和测试。
	// 通用性：可以为不同的类型实现相同的接口，从而在同一上下文中使用它们。例如，除了Person，你可以创建其他类型（如FileWriter）也实现Writer接口，然后在同一代码中使用它们。
    var w Writer = p
    w.Write("Testing interface")
    
    // 使用map
    cache["key"] = 100
    fmt.Println("Map value:", cache["key"])
    
    // 使用defer
    deferExample()
    
    // 使用goto
    gotoExample(number)
    
    // 使用chan和goroutine
	// 在Go语言中，make是一个内置函数，用于创建和初始化切片、映射和通道
	// 初始化通道：make函数不仅创建了通道，还对其进行了初始化，使其可以在程序中使用。通道在创建后可以用于发送和接收数据。
	// 	通道的特点
	// 类型：chan int表示这是一个可以传输整数的通道。通道的类型决定了可以通过该通道发送和接收的数据类型。
	// 阻塞特性：通道的操作是阻塞的，发送和接收操作会在没有对应操作的情况下阻塞，直到有数据可用或接收方准备好接收数据。
	// 用于并发：通道是Go语言中用于在不同goroutine之间进行通信的主要方式。通过通道，goroutine可以安全地交换数据。
	// 使用示例
	// 在创建通道后，可以使用以下方式进行数据传输：
	// 向通道发送数据
	// ch1 <- 42  // 将整数42发送到ch1通道

	// // 从通道接收数据
	// value := <-ch1  // 从ch1通道接收数据并赋值给value
    ch1 := make(chan int)
    ch2 := make(chan int)
    quit := make(chan int)
    
    // 启动goroutine
    go func() {
        for i := 0; i < 2; i++ {
            ch1 <- i
            time.Sleep(200 * time.Millisecond)
        }
    }()
    
    go func() {
        for i := 10; i < 12; i++ {
            ch2 <- i
            time.Sleep(300 * time.Millisecond)
        }
        quit <- 0
    }()
    
    // 使用select处理通道
    selectExample(ch1, ch2, quit)
    
    fmt.Println("Program completed")
}


// Number is positive
// x is greater than 8
// Loop iteration: 0
// Loop iteration: 1
// Loop iteration: 3
// Index: 0, Value: 1
// Index: 1, Value: 2
// Index: 2, Value: 3
// Index: 3, Value: 4
// Index: 4, Value: 5
// Number is 5
// This will also be printed when number is 5
// Gopher writes: Testing interface
// Map value: 100
// Starting defer example
// This will be printed first
// This will be printed last
// Processing 5
// End of goto example
// No channel ready
// Received from channel 2: 10
// Received from channel 1: 0
// No channel ready
// No channel ready
// Received from channel 1: 1
// No channel ready
// Received from channel 2: 11
// No channel ready
// No channel ready
// No channel ready
// Quitting select example
// Program completed