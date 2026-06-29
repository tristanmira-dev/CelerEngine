function (copyAssets TARGET)
    #cmake_parse_arguments()

    set(ASSETS_OUTPUT_DIR $<TARGET_FILE_DIR:${TARGET}>/assets)
	
  add_custom_command (
          TARGET mainShaders POST_BUILD #PLEASE REVISE ON THIS, HAD SOME MAJOR STALE SHADER ISSUE MY GOD
          COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/extern/assets ${ASSETS_OUTPUT_DIR}
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          COMMENT "Copying Assets"
          VERBATIM
  )
	
endfunction()