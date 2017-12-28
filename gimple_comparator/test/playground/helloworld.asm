
a.out:     file format elf64-x86-64


Disassembly of section .init:

00000000004003c8 <_init>:
  4003c8:	48 83 ec 08          	sub    $0x8,%rsp
  4003cc:	48 8b 05 25 0c 20 00 	mov    0x200c25(%rip),%rax        # 600ff8 <__gmon_start__>
  4003d3:	48 85 c0             	test   %rax,%rax
  4003d6:	74 02                	je     4003da <_init+0x12>
  4003d8:	ff d0                	callq  *%rax
  4003da:	48 83 c4 08          	add    $0x8,%rsp
  4003de:	c3                   	retq   

Disassembly of section .plt:

00000000004003e0 <.plt>:
  4003e0:	ff 35 22 0c 20 00    	pushq  0x200c22(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  4003e6:	ff 25 24 0c 20 00    	jmpq   *0x200c24(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  4003ec:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004003f0 <printf@plt>:
  4003f0:	ff 25 22 0c 20 00    	jmpq   *0x200c22(%rip)        # 601018 <printf@GLIBC_2.2.5>
  4003f6:	68 00 00 00 00       	pushq  $0x0
  4003fb:	e9 e0 ff ff ff       	jmpq   4003e0 <.plt>

Disassembly of section .text:

0000000000400400 <_start>:
  400400:	31 ed                	xor    %ebp,%ebp
  400402:	49 89 d1             	mov    %rdx,%r9
  400405:	5e                   	pop    %rsi
  400406:	48 89 e2             	mov    %rsp,%rdx
  400409:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40040d:	50                   	push   %rax
  40040e:	54                   	push   %rsp
  40040f:	49 c7 c0 90 05 40 00 	mov    $0x400590,%r8
  400416:	48 c7 c1 20 05 40 00 	mov    $0x400520,%rcx
  40041d:	48 c7 c7 d7 04 40 00 	mov    $0x4004d7,%rdi
  400424:	ff 15 c6 0b 20 00    	callq  *0x200bc6(%rip)        # 600ff0 <__libc_start_main@GLIBC_2.2.5>
  40042a:	f4                   	hlt    
  40042b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000400430 <deregister_tm_clones>:
  400430:	55                   	push   %rbp
  400431:	b8 28 10 60 00       	mov    $0x601028,%eax
  400436:	48 3d 28 10 60 00    	cmp    $0x601028,%rax
  40043c:	48 89 e5             	mov    %rsp,%rbp
  40043f:	74 17                	je     400458 <deregister_tm_clones+0x28>
  400441:	b8 00 00 00 00       	mov    $0x0,%eax
  400446:	48 85 c0             	test   %rax,%rax
  400449:	74 0d                	je     400458 <deregister_tm_clones+0x28>
  40044b:	5d                   	pop    %rbp
  40044c:	bf 28 10 60 00       	mov    $0x601028,%edi
  400451:	ff e0                	jmpq   *%rax
  400453:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  400458:	5d                   	pop    %rbp
  400459:	c3                   	retq   
  40045a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400460 <register_tm_clones>:
  400460:	be 28 10 60 00       	mov    $0x601028,%esi
  400465:	55                   	push   %rbp
  400466:	48 81 ee 28 10 60 00 	sub    $0x601028,%rsi
  40046d:	48 89 e5             	mov    %rsp,%rbp
  400470:	48 c1 fe 03          	sar    $0x3,%rsi
  400474:	48 89 f0             	mov    %rsi,%rax
  400477:	48 c1 e8 3f          	shr    $0x3f,%rax
  40047b:	48 01 c6             	add    %rax,%rsi
  40047e:	48 d1 fe             	sar    %rsi
  400481:	74 15                	je     400498 <register_tm_clones+0x38>
  400483:	b8 00 00 00 00       	mov    $0x0,%eax
  400488:	48 85 c0             	test   %rax,%rax
  40048b:	74 0b                	je     400498 <register_tm_clones+0x38>
  40048d:	5d                   	pop    %rbp
  40048e:	bf 28 10 60 00       	mov    $0x601028,%edi
  400493:	ff e0                	jmpq   *%rax
  400495:	0f 1f 00             	nopl   (%rax)
  400498:	5d                   	pop    %rbp
  400499:	c3                   	retq   
  40049a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004004a0 <__do_global_dtors_aux>:
  4004a0:	80 3d 7d 0b 20 00 00 	cmpb   $0x0,0x200b7d(%rip)        # 601024 <_edata>
  4004a7:	75 17                	jne    4004c0 <__do_global_dtors_aux+0x20>
  4004a9:	55                   	push   %rbp
  4004aa:	48 89 e5             	mov    %rsp,%rbp
  4004ad:	e8 7e ff ff ff       	callq  400430 <deregister_tm_clones>
  4004b2:	c6 05 6b 0b 20 00 01 	movb   $0x1,0x200b6b(%rip)        # 601024 <_edata>
  4004b9:	5d                   	pop    %rbp
  4004ba:	c3                   	retq   
  4004bb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4004c0:	f3 c3                	repz retq 
  4004c2:	0f 1f 40 00          	nopl   0x0(%rax)
  4004c6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  4004cd:	00 00 00 

00000000004004d0 <frame_dummy>:
  4004d0:	55                   	push   %rbp
  4004d1:	48 89 e5             	mov    %rsp,%rbp
  4004d4:	5d                   	pop    %rbp
  4004d5:	eb 89                	jmp    400460 <register_tm_clones>

00000000004004d7 <main>:
  4004d7:	55                   	push   %rbp
  4004d8:	48 89 e5             	mov    %rsp,%rbp
  4004db:	48 83 ec 10          	sub    $0x10,%rsp
  4004df:	c7 45 fc 02 00 00 00 	movl   $0x2,-0x4(%rbp)
  4004e6:	c7 45 f8 03 00 00 00 	movl   $0x3,-0x8(%rbp)
  4004ed:	8b 45 f8             	mov    -0x8(%rbp),%eax
  4004f0:	2b 45 fc             	sub    -0x4(%rbp),%eax
  4004f3:	89 45 fc             	mov    %eax,-0x4(%rbp)
  4004f6:	8b 45 fc             	mov    -0x4(%rbp),%eax
  4004f9:	89 c6                	mov    %eax,%esi
  4004fb:	bf b0 05 40 00       	mov    $0x4005b0,%edi
  400500:	b8 00 00 00 00       	mov    $0x0,%eax
  400505:	e8 e6 fe ff ff       	callq  4003f0 <printf@plt>
  40050a:	b8 00 00 00 00       	mov    $0x0,%eax
  40050f:	c9                   	leaveq 
  400510:	c3                   	retq   
  400511:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400518:	00 00 00 
  40051b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000400520 <__libc_csu_init>:
  400520:	41 57                	push   %r15
  400522:	41 56                	push   %r14
  400524:	49 89 d7             	mov    %rdx,%r15
  400527:	41 55                	push   %r13
  400529:	41 54                	push   %r12
  40052b:	4c 8d 25 de 08 20 00 	lea    0x2008de(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  400532:	55                   	push   %rbp
  400533:	48 8d 2d de 08 20 00 	lea    0x2008de(%rip),%rbp        # 600e18 <__init_array_end>
  40053a:	53                   	push   %rbx
  40053b:	41 89 fd             	mov    %edi,%r13d
  40053e:	49 89 f6             	mov    %rsi,%r14
  400541:	4c 29 e5             	sub    %r12,%rbp
  400544:	48 83 ec 08          	sub    $0x8,%rsp
  400548:	48 c1 fd 03          	sar    $0x3,%rbp
  40054c:	e8 77 fe ff ff       	callq  4003c8 <_init>
  400551:	48 85 ed             	test   %rbp,%rbp
  400554:	74 20                	je     400576 <__libc_csu_init+0x56>
  400556:	31 db                	xor    %ebx,%ebx
  400558:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  40055f:	00 
  400560:	4c 89 fa             	mov    %r15,%rdx
  400563:	4c 89 f6             	mov    %r14,%rsi
  400566:	44 89 ef             	mov    %r13d,%edi
  400569:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40056d:	48 83 c3 01          	add    $0x1,%rbx
  400571:	48 39 dd             	cmp    %rbx,%rbp
  400574:	75 ea                	jne    400560 <__libc_csu_init+0x40>
  400576:	48 83 c4 08          	add    $0x8,%rsp
  40057a:	5b                   	pop    %rbx
  40057b:	5d                   	pop    %rbp
  40057c:	41 5c                	pop    %r12
  40057e:	41 5d                	pop    %r13
  400580:	41 5e                	pop    %r14
  400582:	41 5f                	pop    %r15
  400584:	c3                   	retq   
  400585:	90                   	nop
  400586:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40058d:	00 00 00 

0000000000400590 <__libc_csu_fini>:
  400590:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000400594 <_fini>:
  400594:	48 83 ec 08          	sub    $0x8,%rsp
  400598:	48 83 c4 08          	add    $0x8,%rsp
  40059c:	c3                   	retq   
