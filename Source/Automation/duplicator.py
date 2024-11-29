import unreal
import os
import time

start_time = time.time()

#instances of unreal classes
system_lib = unreal.SystemLibrary()
editor_util = unreal.EditorUtilityLibrary()
editor_asset_lib = unreal.EditorAssetLibrary()

#get the selected assets
selected_assets = editor_util.get_selected_assets()
num_assets = len(selected_assets)

num_copies = 500
total_num_copies = num_assets * num_copies
text_label = "Duplicating Assets"
running = True

with unreal.ScopedSlowTask(total_num_copies, text_label) as slow_task:
    # display the dialog
    slow_task.make_dialog(True)

    for asset in selected_assets:
        # get the asset name and path to be duplicated
        asset_name = asset.get_fname()
        asset_path = editor_asset_lib.get_path_name_for_loaded_asset(asset)
        sourse_path = os.path.dirname(asset_path)

        for i in range(num_copies):
            #if user pressed cancel bullton, stop
            if slow_task.should_cancel():
                running = False
                break

            new_name = "{}_{}".format(asset_name, i)
            dest_path = os.path.join(sourse_path, new_name)
            duplicate = editor_asset_lib.duplicate_asset(asset_path, dest_path)
            slow_task.enter_progress_frame(1)

            if duplicate is None:
                unreal.log_warning("Duplicate from {} at {} already exists".format(sourse_path, dest_path))

        if not running:
            break

    end_time = time.time()
    unreal.log("{} asset/s duplicated {} times. That took's {}".format(num_assets, num_copies, end_time - start_time))