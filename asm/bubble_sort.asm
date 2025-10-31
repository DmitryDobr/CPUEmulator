.data
array: 5, 4, 3, 6, 1
size: 5

.text
	MOV REG2, [size]      ; размер массива в регистр цикла
	DEC REG2	      ; счет идет с 0я
outer_loop:		      ; подготовка к циклу
	MOV REG3, array       ; адрес на массив в REG3 (первый элемент)
	MOV REG4, 0	      ; i
	MOV REG5, 1	      ; i+1
	MOV REG8, 0	      ; REG8 - флаг того что обмен был произведен
inner_loop:
	MOV REG6, [REG3+REG4] ; array[i], i=REG4
	MOV REG7, [REG3+REG5] ; array[i+1], i+1=REG5
	CMP REG6, REG7	      ;
	JS  no_swap           ; if array[i] <= array[i+1]
	MOV [REG3+REG4], REG7 ; обмен элементов
	MOV [REG3+REG5], REG6 ; обмен элементов
	MOV REG8, 1	      ; REG8 - флаг того что обмен был произведен
no_swap:
	INC REG4 	      ; (i)++
	INC REG5 	      ; (i+1)++
	CMP REG4, REG2	      ; до конца ли массива прошли i < size?
	JS  inner_loop	      ; i < size
	CMP REG8, 0	      ; были обмены?
	JZ done		      ; если нет, то завершаем
	LOOP outer_loop       ; идем дальше по циклам
done:
	HLT		      