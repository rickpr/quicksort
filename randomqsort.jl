function partition(array, initial, last)
  pivot = rand(initial:last)
  array[pivot], array[last] = array[last], array[pivot]
  pivot = array[last]
  start = initial
  for inspector in start:last
    if array[inspector] < pivot
      array[start], array[inspector] = array[inspector], array[start]
      start += 1
    end
  end
  array[start], array[last] = array[last], array[start]
  start
end

function quick_sort(array, start, last)
  if start < last
    pivot_index = partition(array, start, last)
    quick_sort(array, start, pivot_index - 1)
    quick_sort(array, pivot_index + 1, last)
  end
end

srand(BigInt(floor(time())))
for size in [10, 100, 1000, 10000]
  array = Array(Int64, size)
  for array_builder in 1:size
    array[array_builder] = rand(Int64)
  end
  println("\n******* QUICKSORT TIME FOR SIZE $(size) ARRAY *******")
  print("Random: ")
  @time quick_sort(array, 1, size)
  print("Sorted: ")
  @time quick_sort(array, 1, size)
end
