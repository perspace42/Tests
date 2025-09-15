package com.tests.kotlin.spring

import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.RequestMapping;

@RestController
@SpringBootApplication
class Application{

	@RequestMapping("/")
	fun home() = "Hello World";
}

fun main(args: Array<String>) {
	runApplication<Application>(*args)
}
