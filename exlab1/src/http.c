// http.c

#include "http.h"
#define RESPONSE_SIZE (1024)
#define BUFFER_SIZE (1024)
#define PATH_SIZE (1024)
#define SYNC_SIZE (32)

// 请完成这个函数
// 这个函数的参数是一个字符串，表示原始的请求，你可以用printf打印看看是啥
// 这个函数的返回值是response，表示输出的响应，你需要按照格式来自行构造
// 理论上，你写完这个函数，编译运行，能通过访问 http://127.0.0.1 来交互
// 你可以尝试着让你写的函数去访问htdocs里面的网页，方法是通过读取文件
// 如果你不会写，可以试试最暴力地方法，先自己构造一个静态的response在浏览器里看看效果
// 如果你实在不会，可以在answer文件夹里参考答案，然后自己独立实现出来
char *http_process(char *request)
{
	static char response[RESPONSE_SIZE]; 
	static char buffer[BUFFER_SIZE]; 
	static char path[PATH_SIZE]; 
	static char http_version[SYNC_SIZE]; 
	static char method[SYNC_SIZE];  // http请求方法

	*response = '\0';
	char *req_ptr = request;
	int flag = 0; // 0 for method, 1 for uri, 2 for http version， 3 for finished
	char *uri_ptr = path;
	char *method_ptr = method;
	char *http_version_ptr = http_version;

	// 仅解析request的第一行，得到method, uri, http version
	for ( ; flag != 3; req_ptr ++)
	{
		switch (flag)
		{
		case 0:  // method
			if (*req_ptr == ' ')
			{
				*method_ptr = '\0';
				method_ptr = method;
				flag ++;
			}
			else
			{
				*method_ptr ++ = *req_ptr;
			}
			break;
		case 1:  // uri
			if (*req_ptr == ' ')
			{
				*uri_ptr = '\0';
				uri_ptr = path;
				flag ++;
			}
			else
			{
				*uri_ptr ++ = *req_ptr;
			}
			break;
		case 2:  // http version
			if (*req_ptr == '\r' && *(req_ptr + 1) == '\n')
			{
				*http_version_ptr = '\0';
				http_version_ptr = http_version;
				flag ++;
				req_ptr ++;
			}
			else
			{
				*http_version_ptr ++ = *req_ptr;
			}
			break;
		default:
			printf("request prase error: flag = %d\n", flag);
			break;
		}
	}

	printf("request -> method: %s\n", method);
	printf("request -> uri: %s\n", path);
	printf("request -> http version: %s\n", http_version);
	
	// 仅支持GET方法
	if (strcmp(method, "GET") == 0)
	{
		// 处理uri，得到请求的文件路径
		if (strcmp(path, "/") == 0)
		{
			strcpy(path, "/index.html");
		}

		sprintf(buffer, "htdocs%s", path);
		strcpy(path, buffer);
		printf("path: %s\n", path);

		// 读取文件内容，构造response
		FILE *file = fopen(path, "r");
		if (file == NULL)
		{	// 文件不存在, 返回404
			sprintf(response, "%s 404 Not Found\r\n", http_version);
			strcpy(path, "./htdocs/404.html");
			file = fopen(path, "r");
		}
		else
		{	// 文件存在, 返回200ok
			sprintf(response, "%s 200 OK\r\n", http_version);
		}

		sprintf(response + strlen(response),  // 注意这里需加上strlen(response)，否则会覆盖
				"Server: ExLab1\r\n"
				"Connection: close\r\n"
				"Content-type: text/html\r\n\r\n"
		);

		char *response_ptr = response + strlen(response);
		while (fgets(path, BUFFER_SIZE, file) != NULL)
		{
			strcpy(response_ptr, path);
			response_ptr += strlen(path);
		}
		fclose(file);
	}
	else
	{
		sprintf(response, "%s 501 Not Implemented\r\n\r\n", http_version);
	}

	printf("\n====== response =======\n%s\n====================\n", response);
	return response;
}
