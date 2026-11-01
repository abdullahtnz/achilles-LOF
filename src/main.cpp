#include <cstdio>
#include <iostream>
#include "save_manager.hpp"

int main() {
    std::printf("=== Achilles save system — Step 1 smoke test ===\n");

    MetaState meta_snapshot;   // to compare after reopen

    {
        SaveManager save;
        std::printf("Save directory: %s\n", save.SaveDir().c_str());

        MetaState meta = save.LoadMeta();
        std::printf("Loaded: kleos=%llu, deaths=%u, runs=%u, weapons=%zu\n",
                    (unsigned long long)meta.kleos,
                    meta.total_deaths, meta.total_runs,
                    meta.unlocked_weapons.size());

        meta.total_deaths += 1;
        meta.total_runs   += 1;
        meta.kleos        += 100;
        meta.first_launch_seen = true;
        save.SaveMeta(meta);
        std::printf("Saved after bumping.\n");

        RunState run;
        run.layer = 3;
        run.path  = "ares";
        run.patroclus_alive = true;
        run.run_kleos = 250;
        save.SaveRun(run);
        std::printf("Run saved: layer=%d path=%s patroclus=%s\n",
                    run.layer, run.path.c_str(),
                    run.patroclus_alive ? "alive" : "dead");
        std::printf("HasRun? %s\n", save.HasRun() ? "yes" : "no");

        RunState loaded = save.LoadRun();
        std::printf("Run reloaded: layer=%d path=%s kleos=%u\n",
                    loaded.layer, loaded.path.c_str(), loaded.run_kleos);

        save.ClearRun();
        std::printf("Run cleared. HasRun? %s\n", save.HasRun() ? "yes" : "no");

        meta_snapshot = meta;
    }  // <-- `save` closes here, releasing the flock

    // 4. Reopen to prove persistence across close/open.
    {
        SaveManager save2;
        MetaState m2 = save2.LoadMeta();
        std::printf("\nREOPENED: kleos=%llu deaths=%u runs=%u\n",
                    (unsigned long long)m2.kleos,
                    m2.total_deaths, m2.total_runs);
        if (m2.total_deaths == meta_snapshot.total_deaths &&
            m2.kleos        == meta_snapshot.kleos) {
            std::printf("PASS: meta persisted correctly.\n");
        } else {
            std::printf("FAIL: meta did not persist.\n");
        }
    }

    std::printf("\nStep 1 smoke test complete. Clean up with:\n");
    std::printf("  rm -rf ~/.achilles\n");
    return 0;
}