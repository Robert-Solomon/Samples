main()
{
int v = 0;
while((c = getc()) != '\n')
{
v = 10 * v + (c - '0');
}
printf("the value you entered is: %d\n",v);
}
